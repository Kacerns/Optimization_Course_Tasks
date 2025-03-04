import matplotlib.pyplot as plt
import numpy as np

a = 1
b = 8

def f(x):
    return (x**2-a)**2/b-1

Division_Results = []
Golden_Results = []
Newtons_Results = []

with open("Division.txt", "r") as file:
    for line in file:
        Division_Results.extend(map(float, line.split()))

with open("Golden.txt", "r") as file:
    for line in file:
        Golden_Results.extend(map(float, line.split()))

with open("Newtons.txt", "r") as file:
    for line in file:
        Newtons_Results.extend(map(float, line.split()))


# plt.subplot(1, 3, 1)
# x = np.linspace(0, 10, 20000)
# plt.plot(x, f(x), color = 'blue')

# LastPoints = []
# LastPoints.append(Division_Results.pop())
# LastPoints.append(Division_Results.pop())


# plt.scatter(Division_Results, f(np.array(Division_Results)),color = "red", marker='o')
# plt.scatter(LastPoints, f(np.array(LastPoints)),color = "red", marker = 'x')


# plt.subplot(1, 3, 2)
# x = np.linspace(0, 10, 20000)
# plt.plot(x, f(x), color = 'blue')
# LastPoints = []
# LastPoints.append(Golden_Results.pop())
# LastPoints.append(Golden_Results.pop())
# plt.scatter(Golden_Results, f(np.array(Golden_Results)),color = "red", marker='o')
# plt.scatter(LastPoints, f(np.array(LastPoints)),color = "red", marker = 'x')

# plt.subplot(1, 3, 3)
# x = np.linspace(0, 10, 20000)
# plt.plot(x, f(x), color = 'blue')
# LastPoints = []
# LastPoints.append(Newtons_Results.pop())
# plt.scatter(Newtons_Results, f(np.array(Newtons_Results)),color = "red", marker='o')
# plt.scatter(LastPoints, f(np.array(LastPoints)),color = "red", marker = 'x')


plt.subplot(1, 3, 1)
x = np.linspace(0, 2, 20000)
plt.plot(x, f(x), color = 'blue')

LastPoints = []
LastPoints.append(Division_Results.pop())
LastPoints.append(Division_Results.pop())


plt.scatter(Division_Results, f(np.array(Division_Results)),color = "red", marker='|')
plt.scatter(LastPoints, f(np.array(LastPoints)),color = "red", marker = 'x')
plt.ylim(-1.5,0.5)
plt.xlim(0,2)


plt.subplot(1, 3, 2)
x = np.linspace(0, 2, 20000)
plt.plot(x, f(x), color = 'blue')
LastPoints = []
LastPoints.append(Golden_Results.pop())
LastPoints.append(Golden_Results.pop())

plt.scatter(Golden_Results, f(np.array(Golden_Results)),color = "red", marker='|')
plt.scatter(LastPoints, f(np.array(LastPoints)),color = "red", marker = 'x')
plt.ylim(-1.5,0.5)
plt.xlim(0,2)

plt.subplot(1, 3, 3)
x = np.linspace(0, 2, 20000)
plt.plot(x, f(x), color = 'blue')
LastPoints = []
LastPoints.append(Newtons_Results.pop())

plt.scatter(Newtons_Results, f(np.array(Newtons_Results)),color = "red", marker='|')
plt.scatter(LastPoints, f(np.array(LastPoints)),color = "red", marker = 'x')

plt.ylim(-1.5,0.5)
plt.xlim(0,2)
plt.show()
