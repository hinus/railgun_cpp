mport sys
import tokenize

from hivisitor import PrintVisitor, EmitVisitor
from hinode import *
import hiop

DEBUG_PARSE = 1
DEBUG_LEX = 2

DEBUG = DEBUG_PARSE

class Token:
    def __init__(self, tok_num, tok_value):
        self.toknum = tok_num
        self.tokvalue = tok_value

global current_token

def current():
    global current_token
    return current_token

def next(tk):
    toknum, tokvalue, _, _, _ = tk.next()

    # skip all "\n"
    while toknum in (4, 5, 6, 54):
        toknum, tokvalue, _, _, _ = tk.next()

    if DEBUG == DEBUG_LEX:
        print "got token %s : %s" % (toknum, tokvalue)

    global current_token
    current_token = Token(toknum, tokvalue)

def match(tk, toknum, tokvalue, is_try = False):
    if toknum == current().toknum and tokvalue == current().tokvalue:
        next(tk)
        return True
    # if is_try, we let the parser continue.
    elif is_try:
        return False
    else:
        raise AssertionError, "mismatch (%s, %s) vs (%s, %s)" % (toknum, tokvalue, current().toknum, current().tokvalue)

def stmts(tk):
    l = ListNode()
    toknum = current().toknum
    tokvalue = current().tokvalue

    while toknum != 0 and tokvalue != "}":
        if tokvalue == "if":
            l.append(if_stmt(tk))
        elif tokvalue == "print":
            l.append(print_stmt(tk))
        elif tokvalue == "while":
            l.append(while_stmt(tk))
        elif tokvalue == "continue":
            l.append(continue_stmt(tk))
        elif tokvalue == "break":
            l.append(break_stmt(tk))
        else:
            l.append(test(tk))

        toknum = current().toknum
        tokvalue = current().tokvalue

        if DEBUG == DEBUG_PARSE:
            print "complete a statement"
            print "debug next stmt: %s, %s" % (toknum, tokvalue)

    if DEBUG == DEBUG_PARSE:
        print l

    return l

def while_stmt(tk):
    match(tk, 1, "while")
    cond = condition(tk)
    body = stmts_block(tk)

    return WhileNode(cond, body)

def continue_stmt(tk):
    match(tk, 1, "continue")
    match(tk, tokenize.OP, ";")
    return ContinueNode()

def break_stmt(tk):
    match(tk, 1, "break")
    match(tk, tokenize.OP, ";")
    return BreakNode()

def if_stmt(tk):
    match(tk, 1, "if")
    cond = condition(tk)
    then_path = stmts_block(tk)

    toknum = current().toknum
    tokvalue = current().tokvalue

    else_path = None

    if match(tk, tokenize.NAME, "else", True):
        if DEBUG == DEBUG_PARSE:
            print "Got an else branch"
        else_path = stmts_block(tk)

    return IfNode(cond, then_path, else_path)

def stmts_block(tk):
    match(tk, tokenize.OP, "{")
    statements = stmts(tk)
    match(tk, tokenize.OP, "}")
    return statements

def condition(tk):
    match(tk, tokenize.OP, "(")
    left = expr(tk)
    comparator = -1
    if match(tk, tokenize.OP, "==", True):
        comparator = hiop.CMP_EQUAL
    elif match(tk, tokenize.OP, "<", True):
        comparator = hiop.CMP_LESS_THAN
    else:
        raise AssertionError, "Unrecognized comparator"

    right = expr(tk)
    match(tk, tokenize.OP, ")")

    return ConditionNode(left, right, comparator)
def print_stmt(tk):
    match(tk, 1, "print")
    value = expr(tk)
    if DEBUG == DEBUG_PARSE:
        print "print value is %s " % value
    match(tk, tokenize.OP, ";")
    return PrintNode(value)

def test(tk):
    value = expr(tk)

    if match(tk, tokenize.OP, "=", True):
        if isinstance(value, NameNode):
            print "got an assign statement"
            value.context = NameNode.STORE
        else:
            raise AssertionError, "left value should only be NameNode"

        value = AssignNode(value, expr(tk))

    match(tk, tokenize.OP, ";")
    return value

def expr(tk):
    s1 = term(tk)
    toknum = current().toknum
    tokvalue = current().tokvalue

    if tokvalue == "+" or tokvalue == "-":
        if DEBUG == DEBUG_PARSE:
            print "expr tokvalue is %s, left is %s" % (tokvalue, s1)
        result = subexpr(s1, tk)
        if DEBUG == DEBUG_PARSE:
            print "result of subexpr is %s" % result
        return result
    else:
        return s1


def subexpr(left, tk):
    toknum = current().toknum
    tokvalue = current().tokvalue

    value = left

    if tokvalue == "+":
        next(tk)
        value = BinaryOpNode(BinaryOpNode.ADD, left, term(tk))
    elif tokvalue == "-":
        next(tk)
        value = BinaryOpNode(BinaryOpNode.SUB, left, term(tk))

    tokvalue = current().tokvalue
    if tokvalue == "+" or tokvalue == "-":
        return subexpr(value, tk)
    else:
        return value


def term(tk):
    f1 = factor(tk)
    toknum = current().toknum
    tokvalue = current().tokvalue

    if tokvalue == "*" or tokvalue == "/":
        return subterm(f1, tk)
    else:
        return f1

def subterm(left, tk):
    toknum = current().toknum
    tokvalue = current().tokvalue

    value = left

    if tokvalue == "*":
        next(tk)
        value = BinaryOpNode(BinaryOpNode.MUL, left, factor(tk))
    elif tokvalue == "/":
        next(tk)
        value = BinaryOpNode(BinaryOpNode.DIV, left, factor(tk))
    tokvalue = current().tokvalue
    if tokvalue == "*" or tokvalue == "/":
        return subterm(value, tk)

    else:
        return value

def factor(tk):
    if current().toknum == tokenize.NUMBER:
        value = current().tokvalue
        next(tk)
        return ConstNode(int(value))

    elif current().toknum == tokenize.STRING:
        value = current().tokvalue
        next(tk)
        return ConstNode(value[1:-1])

    elif current().toknum == tokenize.NAME:
        if DEBUG == DEBUG_PARSE:
            print "got a variable"
        value = current().tokvalue
        next(tk)
        return NameNode(value, NameNode.LOAD)

    else:
        match(tk, tokenize.OP, "(")
        value = expr(tk)
        match(tk, tokenize.OP, ")")
        return value


if __name__ == "__main__":
    f = open(sys.argv[1])
    tk = tokenize.generate_tokens(f.readline)
    next(tk)
    #PrintVisitor().visit(stmts(tk))
    EmitVisitor("%sc" % sys.argv[1]).visit(stmts(tk))