import matplotlib.pyplot as plt
import numpy as np

a = 1
b = 8

def f(x):
    return (x**2-a)**2/b-1


x = np.linspace(0, 2, 1000)

print(x)

plt.plot(x, f(x), color = 'red')

plt.show()