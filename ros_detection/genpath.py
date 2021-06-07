import numpy as np
import matplotlib.pyplot as plt
import math
from nav_msgs.msg import Path
from std_msgs.msg import Header
from geometry_msgs.msg import PoseStamped
x0 = -10.5 #m
v0 = 1.8 #m/s
a = 0 #m/s^2
t = np.linspace(0,40,200)
global path
path = Path()
global position
position = 0
def f(A, w, phi, t):
    position = position +  A*math.sin(w*t + phi)
    return position
# x = f(x0, v0, a, t)
# plt.plot(t,x)
# plt.show()
#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import Float64

def talker(t0):
    t_current = rospy.get_time()
    t = t_current - t0
    x0 = -10.5 #m
    a = 0 #m/s^2
    x = Float64()
    A = 100
    w = 0.9
    phi = 0
    x.data = f(A,w,phi, t)
    pub = rospy.Publisher('chatter', Float64, queue_size=10)
    pub2 = rospy.Publisher("person_path", Path, queue_size=10)
    header = Header()
    path.header.stamp = rospy.Time.now()
    path.header.frame_id = "map"
    header.stamp = rospy.Time.now()
    header.frame_id = "map"
    posestamped = PoseStamped()
    posestamped.pose.position.x = x.data
    posestamped.pose.position.y = -4
    posestamped.pose.position.z = 0.386999999123
    posestamped.pose.orientation.w = 1
    posestamped.header = header


    path.poses.append(posestamped)
    pub2.publish(path)

    rate = rospy.Rate(10) # 10hz
    # rospy.loginfo(x)
    pub.publish(x)
    rate.sleep()

if __name__ == '__main__':
    try:
        rospy.init_node('talker', anonymous=True)
        t0 = rospy.get_time()
        while True:
            talker(t0)
    except rospy.ROSInterruptException:
        pass
