from z3 import *
import time
import random
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import matplotlib.cm as cm


class MyRectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height


def visualize(big_rectangle, small_rectangles, positions, pos, title):
    ax = plt.subplot(pos)
    cmap = cm.get_cmap('gist_rainbow')
    for i, rectangle in enumerate(small_rectangles):
        position = positions[i]
        color = i / len(small_rectangles)
        ax.add_patch(Rectangle((position[0], position[1]), rectangle.width,
                               rectangle.height, alpha=0.5, fill=True, edgecolor='black', facecolor=cmap(color)))
    ax.set_xlim([0, big_rectangle.width])
    ax.set_ylim([0, big_rectangle.height])
    ax.set_xlabel('Width')
    ax.set_ylabel('Height')
    ax.set_title(title)


def z3_rectangle(big_rectangle, small_rectangles):
    x = [Int("x_%s" % (i + 1)) for i in range(len(small_rectangles))]
    y = [Int("y_%s" % (i + 1)) for i in range(len(small_rectangles))]
    w = [Int("w_%s" % (i + 1)) for i in range(len(small_rectangles))]
    h = [Int("h_%s" % (i + 1)) for i in range(len(small_rectangles))]
    constraints = []
    for i in range(len(small_rectangles)):
        # Configuration of small rectangles
        constraints.append(Or(And(w[i] == small_rectangles[i].width, h[i] == small_rectangles[i].height), And(
            w[i] == small_rectangles[i].height, h[i] == small_rectangles[i].width)))
        # Position of small rectangles
        constraints.append(And(x[i] >= 0, y[i] >= 0, x[i] + w[i] <=
                           big_rectangle.width, y[i] + h[i] <= big_rectangle.height))
        # No overlap
        for j in range(i):
            constraints.append(Or(x[i] + w[i] <= x[j], x[j] + w[j]
                               <= x[i], y[i] + h[i] <= y[j], y[j] + h[j] <= y[i]))
    time1 = time.time()
    solver = Solver()
    solver.add(constraints)
    if solver.check() == sat:
        m = solver.model()
        for i in range(len(small_rectangles)):
            print("x_%s = %s, y_%s = %s, w_%s = %s, h_%s = %s" % (
                i + 1, m[x[i]], i + 1, m[y[i]], i + 1, m[w[i]], i + 1, m[h[i]]))
        time2 = time.time()
        print(time2 - time1)
        visualize(big_rectangle, [MyRectangle(m[w[i]].as_long(), m[h[i]].as_long()) for i in range(len(small_rectangles))],
                  [(m[x[i]].as_long(), m[y[i]].as_long()) for i in range(len(small_rectangles))], 121, "Z3")
    else:
        print("No solution")
        time2 = time.time()
        print(time2 - time1)


class Position:
    def __init__(self, x, y):
        self.x = x
        self.y = y


def overlap(rectangle1, rectangle2, position1, position2):
    left1 = position1.x
    right1 = position1.x + rectangle1.width
    top1 = position1.y
    bottom1 = position1.y + rectangle1.height
    left2 = position2.x
    right2 = position2.x + rectangle2.width
    top2 = position2.y
    bottom2 = position2.y + rectangle2.height
    return not (left1 >= right2 or right1 <= left2 or top1 >= bottom2 or bottom1 <= top2)


def my_rectangle(big_rectangle, small_rectangles):
    def find_recursion(big_rectangle, small_rectangles, positions, n, result_rectangles):
        rectangle = small_rectangles[n]
        for x in range(big_rectangle.width):
            for y in range(big_rectangle.height):
                position = Position(x, y)
                if x + rectangle.width <= big_rectangle.width and y + rectangle.height <= big_rectangle.height:
                    overlaps = False
                    for i in range(len(positions)):
                        if overlap(rectangle, result_rectangles[i], position, positions[i]):
                            overlaps = True
                            break
                    if not overlaps:
                        positions.append(position)
                        result_rectangles.append(rectangle)
                        if n == len(small_rectangles) - 1:
                            return True
                        else:
                            find = find_recursion(
                                big_rectangle, small_rectangles, positions, n + 1, result_rectangles)
                            if not find:
                                positions.pop()
                                result_rectangles.pop()
                            else:
                                return True
                if x + rectangle.height <= big_rectangle.width and y + rectangle.width <= big_rectangle.height:
                    overlaps = False
                    for i in range(len(positions)):
                        if overlap(MyRectangle(small_rectangles[i].height, small_rectangles[i].width), result_rectangles[i], position, positions[i]):
                            overlaps = True
                            break
                    if not overlaps:
                        positions.append(position)
                        result_rectangles.append(MyRectangle(
                            rectangle.height, rectangle.width))
                        if n == len(small_rectangles) - 1:
                            return True
                        else:
                            find = find_recursion(
                                big_rectangle, small_rectangles, positions, n + 1, result_rectangles)
                            if not find:
                                positions.pop()
                                result_rectangles.pop()
                            else:
                                return True

    time1 = time.time()
    positions = []
    small_rectangles = sorted(
        small_rectangles, key=lambda r: r.width * r.height, reverse=True)
    result_rectangles = []
    find_recursion(big_rectangle, small_rectangles,
                   positions, 0, result_rectangles)

    if len(positions) == len(small_rectangles):
        for i in range(len(small_rectangles)):
            print("x_%s = %s, y_%s = %s, w_%s = %s, h_%s = %s" % (
                i + 1, positions[i].x, i + 1, positions[i].y, i + 1, result_rectangles[i].width, i + 1, result_rectangles[i].height))
        time2 = time.time()
        print(time2 - time1)
        visualize(big_rectangle, result_rectangles, [
                  (positions[i].x, positions[i].y) for i in range(len(positions))], 122, "My")
    else:
        print("No solution")
        time2 = time.time()
        print(time2 - time1)

# 大矩形50个单位乘以55个单位，50个小矩形：

# 矩形1: 18x10
# 矩形2: 5x8
# 矩形3: 7x19
# 矩形4: 16x17
# 矩形5: 20x6
# 矩形6: 2x18
# 矩形7: 3x18
# 矩形8: 7x7
# 矩形9: 14x13
# 矩形10: 17x13
# 矩形11: 9x18
# 矩形12: 19x7
# 矩形13: 16x12
# 矩形14: 9x1
# 矩形15: 4x19
# 矩形16: 19x13
# 矩形17: 10x12
# 矩形18: 12x14
# 矩形19: 5x6
# 矩形20: 5x2


big_rectangle = MyRectangle(50, 55)
small_rectangles = [MyRectangle(18, 10), MyRectangle(5, 8), MyRectangle(7, 19), MyRectangle(16, 17), MyRectangle(20, 6), MyRectangle(2, 18), MyRectangle(3, 18), MyRectangle(7, 7), MyRectangle(14, 13), MyRectangle(17, 13), MyRectangle(9, 18), MyRectangle(19, 7), MyRectangle(16, 12), MyRectangle(9, 1), MyRectangle(4, 19), MyRectangle(19, 13), MyRectangle(10, 12), MyRectangle(12, 14), MyRectangle(5, 6), MyRectangle(5, 2)]

plt.figure(figsize=(12, 5))
z3_rectangle(big_rectangle, small_rectangles)
my_rectangle(big_rectangle, small_rectangles)
plt.show()
