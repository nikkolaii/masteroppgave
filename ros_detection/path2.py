#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Point
from nav_msgs.msg import Path
from std_msgs.msg import Header
from geometry_msgs.msg import PoseStamped
path = Path()
def callback(data):
    global path

    x = data.x
    y = data.y
    z = data.z
    header = Header()
    path.header.stamp = rospy.Time.now()
    path.header.frame_id = "base_link"
    header.stamp = rospy.Time.now()
    header.frame_id = "odom"
    posestamped = PoseStamped()
    posestamped.pose.position = data
    posestamped.pose.orientation.x = 0
    posestamped.pose.orientation.y = 0
    posestamped.pose.orientation.z = 0
    posestamped.pose.orientation.w = 1
    posestamped.header = header


    path.poses.append(posestamped)
    pub.publish(path)
    print("l")
    

rospy.init_node('nenode', anonymous=True)
pub = rospy.Publisher("fitte", Path, queue_size = 10)
rospy.Subscriber("person_position", Point, callback)

# spin() simply keeps python from exiting until this node is stopped
rospy.spin()

if __name__ == '__main__':
    listener()
