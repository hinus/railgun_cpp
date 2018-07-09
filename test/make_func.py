def fico(x):
    def foo(t = x):
        return 1 + t
    return foo

g = fico(1)
f = fico(2)

print g(1)
print f(2)
