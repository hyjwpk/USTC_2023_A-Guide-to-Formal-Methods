from z3 import *

def add():
    a_num = 13
    b_num = 7
    d_num = 0

    n = 5

    a = [Bool("a_%s" % i) for i in range(n)]
    b = [Bool("b_%s" % i) for i in range(n)]
    d = [Bool("d_%s" % i) for i in range(n)]
    c = [Bool("c_%s" % i) for i in range(n + 1)]
    constraints = [d[i] == (a[i] == (b[i] == c[i])) for i in range(n)]
    constraints += [c[i + 1] == (Or(And(a[i], b[i]), And(a[i], c[i]), And(b[i], c[i]))) for i in range(n)]
    constraints += [Not(c[n])]
    constraints += [Not(c[0])]
    for i in range(n):
        if a_num & (1 << i):
            constraints += [a[i]]
        else:
            constraints += [Not(a[i])]
        if b_num & (1 << i):
            constraints += [b[i]]
        else:
            constraints += [Not(b[i])]

    solver = Solver()
    solver.add(constraints)
    if solver.check() == sat:
        m = solver.model()
        for i in range (n):
            if m[d[i]]:
                d_num += 1 << i
        print("d_num = %s" % d_num)

def minus():
    a_num = 13
    b_num = 0
    d_num = 20

    n = 5

    a = [Bool("a_%s" % i) for i in range(n)]
    b = [Bool("b_%s" % i) for i in range(n)]
    d = [Bool("d_%s" % i) for i in range(n)]
    c = [Bool("c_%s" % i) for i in range(n + 1)]
    constraints = [d[i] == (a[i] == (b[i] == c[i])) for i in range(n)]
    constraints += [c[i + 1] == (Or(And(a[i], b[i]), And(a[i], c[i]), And(b[i], c[i]))) for i in range(n)]
    constraints += [Not(c[n])]
    constraints += [Not(c[0])]
    for i in range(n):
        if a_num & (1 << i):
            constraints += [a[i]]
        else:
            constraints += [Not(a[i])]
        if d_num & (1 << i):
            constraints += [d[i]]
        else:
            constraints += [Not(d[i])]

    solver = Solver()
    solver.add(constraints)
    if solver.check() == sat:
        m = solver.model()
        for i in range (n):
            if m[b[i]]:
                b_num += 1 << i
        print("b_num = %s" % b_num)

minus()
add()