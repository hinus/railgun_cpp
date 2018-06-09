from hinode import *
import hiop
import struct

class PrintVisitor():
    def __init__(self):
        self.jump_label = 0

    def visit(self, node):
        node.accept(self)

    def visitIf(self, if_node):
        if_node.cond.accept(self)
        if_node.then_path.accept(self)

        print "%s:" % (self.jump_label)
        self.jump_label += 1

        if_node.else_path.accept(self)

    def visitWhile(self, while_node):
        print "  SETUP_LOOP"
        while_node.cond.accept(self)
        while_node.body.accept(self)
        print "  JUMP_ABSOLUTE"

    def visitList(self, list_node):
        for item in list_node:
            item.accept(self)

    def visitPrint(self, print_node):
        print_node.value.accept(self)
        print "  PRINT"
        print "  PRINT_NEWLINE"

    def visitConst(self, const_node):
        print "  LOAD_CONST %s" % (const_node.value)

    def visitName(self, name_node):
        if name_node.context == NameNode.LOAD:
            print "  LOAD_NAME %s" % (name_node.id)
        else:
            print "  STORE_NAME %s" % (name_node.id)

    def visitAssign(self, assign_node):
        assign_node.right.accept(self)
        assign_node.left.accept(self)


    def visitBinaryOp(self, bin_node):
        bin_node.left.accept(self)
        bin_node.right.accept(self)

        if bin_node.op == BinaryOpNode.ADD:
            print "  ADD"
        elif bin_node.op == BinaryOpNode.SUB:
            print "  SUB"
        elif bin_node.op == BinaryOpNode.MUL:
            print "  MUL"
        elif bin_node.op == BinaryOpNode.DIV:
            print "  DIV"

    def visitCondition(self, cond_node):
        cond_node.left.accept(self)
        cond_node.right.accept(self)
        print "  CAMPARE_OP (==)"
        print "  JMP_IF_FALSE %s" % (self.jump_label)


# code generator
class EmitVisitor():
    def __init__(self, filename):
        self.jump_label = 0
        self.consts = []
        self.names = []
        self.codes = []
        self.loop_begins = []
        self.filename = filename

    def find_const(self, const_value):
        if const_value in self.consts:
            return self.consts.index(const_value)
        else:
            self.consts.append(const_value)
            return len(self.consts) - 1
    def find_name(self, name_value):
        if name_value in self.names:
            return self.names.index(name_value)
        else:
            self.names.append(name_value)
            return len(self.names) - 1

    def visit(self, node):
        node.accept(self)
        f = open(self.filename, "wb")
        self.write_list_to_file(self.consts, f)
        self.write_list_to_file(self.names, f)

        f.write(struct.pack("i", len(self.codes)))
        f.write("".join(self.codes))
        f.close()

    def write_list_to_file(self, l, f):
        f.write(struct.pack("B", len(l)))
        for item in l:
            if type(item) == int:
                f.write(struct.pack("B", 0))
                f.write(struct.pack("i", item))
            elif type(item) == str:
                f.write(struct.pack("B", 1))
                f.write(struct.pack("i", len(item)))
                f.write(struct.pack("%ss" % len(item), item))

    def jump_if_false(self, label):
        self.codes.append(struct.pack("B", hiop.JMP_IF_FALSE))
        label.mark()

    def jump_forward(self, label):
        self.codes.append(struct.pack("B", hiop.JMP_FORWARD))
        label.mark()

    def jump_absolute(self, label):
        self.codes.append(struct.pack("B", hiop.JMP_ABSOLUTE))
        self.codes.append(struct.pack("B", label.pc))

    def visitIf(self, if_node):
        if_node.cond.accept(self)

        label = Label(self.codes)
        end = Label(self.codes)
        self.jump_if_false(label)

        self.codes.append(struct.pack("B", hiop.POP_TOP))
        if_node.then_path.accept(self)

        self.jump_forward(end)

        # else
        label.bind()
        self.codes.append(struct.pack("B", hiop.POP_TOP))
        if if_node.else_path is not None:
            if_node.else_path.accept(self)

        end.bind()

    def setup_loop(self, end_label):
        self.codes.append(struct.pack("B", hiop.SETUP_LOOP))
        end_label.mark()


    def visitWhile(self, while_node):
        loop_begin = Label(self.codes)
        self.loop_begins.append(loop_begin)
        loop_end = Label(self.codes)
        cond_end = Label(self.codes)

        self.setup_loop(loop_end)
        loop_begin.bind()

        while_node.cond.accept(self)
        self.jump_if_false(cond_end)
        self.codes.append(struct.pack("B", hiop.POP_TOP))

        while_node.body.accept(self)
        self.jump_absolute(loop_begin)

        cond_end.bind()
        self.codes.append(struct.pack("B", hiop.POP_TOP))
        self.codes.append(struct.pack("B", hiop.POP_BLOCK))
        loop_end.bind()
        self.loop_begins.pop()

    def visitBreak(self, break_node):
        self.codes.append(struct.pack("B", hiop.BREAK_LOOP))

    def visitContinue(self, cont_node):
        self.jump_absolute(self.loop_begins[-1])

    def visitList(self, list_node):
        for item in list_node:
            item.accept(self)

    def visitPrint(self, print_node):
        print_node.value.accept(self)
        self.codes.append(struct.pack("B", hiop.PRINT_ITEM))
        self.codes.append(struct.pack("B", hiop.PRINT_NEWLINE))

    def visitConst(self, const_node):
        self.codes.append(struct.pack("B", hiop.LOAD_CONST))
        self.codes.append(struct.pack("B", self.find_const(const_node.value)))

    def visitName(self, name_node):
        if name_node.context == NameNode.LOAD:
            self.codes.append(struct.pack("B", hiop.LOAD_NAME))
            self.codes.append(struct.pack("B", self.find_name(name_node.id)))
        else:
            self.codes.append(struct.pack("B", hiop.STORE_NAME))
            self.codes.append(struct.pack("B", self.find_name(name_node.id)))


    def visitAssign(self, assign_node):
        assign_node.right.accept(self)
        assign_node.left.accept(self)


    def visitBinaryOp(self, bin_node):
        bin_node.left.accept(self)
        bin_node.right.accept(self)

        if bin_node.op == BinaryOpNode.ADD:
            self.codes.append(struct.pack("B", hiop.ADD))
        elif bin_node.op == BinaryOpNode.SUB:
            self.codes.append(struct.pack("B", hiop.SUB))
        elif bin_node.op == BinaryOpNode.MUL:
            self.codes.append(struct.pack("B", hiop.MUL))
        elif bin_node.op == BinaryOpNode.DIV:
            self.codes.append(struct.pack("B", hiop.DIV))


    def visitCondition(self, cond_node):
        cond_node.left.accept(self)
        cond_node.right.accept(self)
        self.codes.append(struct.pack("B", hiop.COMPARE_OP))
        self.codes.append(struct.pack("B", cond_node.comparator))

class Label():
    def __init__(self, codes):
        self.pcs = []
        self.codes = codes
        self.pc = 0

    def bind(self):
        self.pc = len(self.codes)

        for ipc in self.pcs:
            self.codes[ipc] = struct.pack("B", self.pc - ipc - 1)

    def mark(self):
        self.pcs.append(len(self.codes))
        self.codes.append(0)
