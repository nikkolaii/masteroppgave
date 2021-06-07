import numpy as np
import matplotlib.pyplot as plt
from nav_msgs.msg import Path
from std_msgs.msg import Header
from geometry_msgs.msg import PoseStamped
x0 = -10.5 #m
v0 = 1.8 #m/s
a = 0 #m/s^2



def f(x0, v0, a, t):

    position = x0 + v0*t + 0.5*a*t**2
    print("v0*t:", v0, t)
    print("\n position", position)
    return position
t = np.linspace(0,40,2000)

x = np.zeros((len(t),1))


for i in range(len(t)):
    x[i] = f(x0,v0,a,t[i])
plt.plot(t,x)
plt.show()

