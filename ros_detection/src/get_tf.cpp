/* Created by Nikolai Persen 2021 */

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/transform_broadcaster.h"
#include <tf/transform_listener.h>
#include <std_msgs/Float64.h>
#include <cmath>
class getTF
{
public:
getTF(){
  dist_sub = nh.subscribe("/Optimized_person_distance",1, &getTF::callback, this);
  path_pub = nh.advertise<nav_msgs::Path>("person_path",100, true);
  _pub2 = nh.advertise<geometry_msgs::Point>("move_base_goal_topic",100,true);
  anglePub_ = nh.advertise<std_msgs::Float64>("FOV_angle", 100, true);
}

void callback(const geometry_msgs::Point::ConstPtr& pointData)
{

  /* Creating a message containing the depth measurements relative to the person_detection_frame
     Note that y = 0 such that the person is confined to the 2D floor plane at the height of the camera */
  personpose.pose.position.x = pointData->x;
  personpose.pose.position.y = 0;
  personpose.pose.position.z = pointData->z;
  personpose.pose.orientation.w = 1;
  personpose.header.stamp = ros::Time(0);
  personpose.header.frame_id = "person_detection_frame";
  std_msgs::Float64 FOV_angle;
  FOV_angle.data  = -std::atan2(pointData->z, pointData->x) * 180 / 3.141592 +90;

ROS_INFO_STREAM(FOV_angle);
/* Listening for a transform from person_detection_frame to the intertial map frame */
  _listener.waitForTransform("map","person_detection_frame",ros::Time::now(),ros::Duration(3.0));
  _listener.transformPose("map", personpose, mappose);

/* Broadcasting the coordinate frame "person" relative to the inertial map frame */
  _transform.setRotation(tf::Quaternion(mappose.pose.orientation.x,mappose.pose.orientation.y,mappose.pose.orientation.z,mappose.pose.orientation.w));
  _transform.setOrigin(tf::Vector3(mappose.pose.position.x, mappose.pose.position.y, mappose.pose.position.z));
  _br.sendTransform(tf::StampedTransform(_transform, ros::Time::now(), "map", "person"));

/* Defining the path message for the path publisher */
path.header.stamp = ros::Time::now();
path.header.frame_id = "map";
posestamped.header.stamp = ros::Time::now();
posestamped.header.frame_id = "map";
posestamped.pose.position = mappose.pose.position;
posestamped.pose.orientation = mappose.pose.orientation;
path.poses.push_back(posestamped);
path_pub.publish(path);
_pub2.publish(posestamped.pose.position);
anglePub_.publish(FOV_angle);
}

private:
  ros::NodeHandle nh;
  ros::Subscriber dist_sub;
  ros::Publisher path_pub;
  ros::Publisher _pub2;
  ros::Publisher anglePub_;

  tf::TransformBroadcaster _br;
  tf::Transform _transform;
  tf::Quaternion quat;
  tf::TransformListener _listener;

  geometry_msgs::PoseStamped personpose;
  geometry_msgs::PoseStamped mappose;
  geometry_msgs::PoseStamped posestamped;
  nav_msgs::Path path;
};

int main(int argc, char **argv)
{
  ros::init(argc,argv,"node");
  getTF tfobj;
  ros::spin();
  return 0;
}
