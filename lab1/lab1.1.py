from z3 import *
import time
import xlwt


def solve_n_queens_sat(n):
    board = [[Bool("x_%s_%s" % (i, j)) for j in range(n)] for i in range(n)]
    # 行、列约束
    constraints = [Or([board[i][j] for j in range(n)]) for i in range(n)]
    constraints += [Or(Not(board[i][j]), Not(board[i][k])) for i in range(n) for j in range(n) for k in
                    range(j + 1, n)]
    constraints += [Or([board[i][j] for i in range(n)]) for j in range(n)]
    constraints += [Or(Not(board[i][j]), Not(board[k][j])) for j in range(n) for i in range(n) for k in
                    range(i + 1, n)]
    # 对角线约束
    for i1 in range(n):
        for i2 in range(i1+1, n):
            for j1 in range(i2-i1, n):
                j2 = i1 + j1 - i2
                constraints += [Or(Not(board[i1][j1]), Not(board[i2][j2]))]
            for j1 in range(n+i1-i2):
                j2 = i2 - i1 + j1
                constraints += [Or(Not(board[i1][j1]), Not(board[i2][j2]))]

    solver = Solver()
    solver.add(constraints)
    # 输出k个解
    time1 = time.time()
    for k in range(1):
        if solver.check() == sat:
            model = solver.model()
            solution = []
            for i in range(n):
                for j in range(n):
                    if model[board[i][j]]:
                        solution += [j]
            print(solution)

            # 将当前解添加进约束以求出新的解
            solver.add(Or([Not(board[i][solution[i]]) for i in range(n)]))
        else:
            print(None)
    time2 = time.time()
    return time2 - time1


def solve_n_queens_smt(n):
    Q = [Int('Q_ % i' % (i + 1)) for i in range(n)]
    val_c = [And(1 <= Q[i], Q[i] <= n) for i in range(n)]
    col_c = [Distinct(Q)]
    diag_c = [If(i == j, True,
                 And(i + Q[i] != j + Q[j], i + Q[j] != j + Q[i]))
              for i in range(n) for j in range(i)]
    solver = Solver()
    solver.add(val_c + col_c + diag_c)
    time1 = time.time()
    if solver.check() == sat:
        model = solver.model()
        print(model)
    else:
        print(None)
    time2 = time.time()
    return time2 - time1


def time_calculate_with_constrain():
    # 时间统计
    workbook = xlwt.Workbook(encoding='utf-8')
    sheet = workbook.add_sheet('sheet_1')
    sheet.write(0, 0, 'n')
    sheet.write(0, 1, 'smt')
    sheet.write(0, 2, 'sat')

    for i in range(1, 41):
        print(i)
        time1 = time.time()
        solve_n_queens_smt(i)
        time2 = time.time()

        time3 = time.time()
        solve_n_queens_sat(i)
        time4 = time.time()

        # 输出到excel
        sheet.write(i, 0, i)
        sheet.write(i, 1, time2 - time1)
        sheet.write(i, 2, time4 - time3)
        workbook.save('C:\\Users\\wangc\\Desktop\\time.xls')

def time_calculate_without_constrain():
    # 时间统计
    workbook = xlwt.Workbook(encoding='utf-8')
    sheet = workbook.add_sheet('sheet_1')
    sheet.write(0, 0, 'n')
    sheet.write(0, 1, 'smt')
    sheet.write(0, 2, 'sat')

    for i in range(1, 31):
        print(i)
        time1 = solve_n_queens_smt(i)
        time2 = solve_n_queens_sat(i)

        # 输出到excel
        sheet.write(i, 0, i)
        sheet.write(i, 1, time1)
        sheet.write(i, 2, time2)
        workbook.save('C:\\Users\\wangc\\Desktop\\time.xls')


time1 = time.time()
solve_n_queens_sat(8)
time2 = time.time()
print(time2 - time1)