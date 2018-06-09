
class IfNode():
    def __init__(self, cond, then_path, else_path):
        self.cond = cond
        self.then_path = then_path
        self.else_path = else_path

    def accept(self, visitor):
        visitor.visitIf(self)

class WhileNode():
    def __init__(self, cond, body):
        self.cond = cond
        self.body = body

    def accept(self, visitor):
        visitor.visitWhile(self)

class ContinueNode():
    def __init__(self):
        pass

    def accept(self, visitor):
        visitor.visitContinue(self)

class BreakNode():
    def __init__(self):
        pass

    def accept(self, visitor):
        visitor.visitBreak(self)

class PrintNode():
    def __init__(self, value):
        self.value = value

    def accept(self, visitor):
        visitor.visitPrint(self)

class ConditionNode():
    def __init__(self, left, right, comparator):
        self.left = left
        self.right = right
        self.comparator = comparator

    def accept(self, visitor):
        visitor.visitCondition(self)


class ListNode(list):
    def __init__(self):
        pass

    def accept(self, visitor):
        visitor.visitList(self)

class ConstNode():
    def __init__(self, value):
        self.value = value

    def accept(self, visitor):
        visitor.visitConst(self)

class BinaryOpNode():
    ADD = 0
    SUB = 1
    MUL = 2
    DIV = 3
    def __init__(self, op, left, right):
        self.op = op
        self.left = left
        self.right = right

    def accept(self, visitor):
        visitor.visitBinaryOp(self)

class NameNode():
    UNKNOWN = 0
    STORE = 1
    LOAD = 2

    def __init__(self, id, context):
        self.id = id
        self.context = context

    def accept(self, visitor):
        visitor.visitName(self)

class AssignNode():
    def __init__(self, left, right):
        self.left = left
        self.right = right

    def accept(self, visitor):
        visitor.visitAssign(self)
