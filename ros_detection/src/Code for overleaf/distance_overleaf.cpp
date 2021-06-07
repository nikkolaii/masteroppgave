/* Created by Nikolai Persen 2021 */

#include "ros/ros.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/transform_broadcaster.h"
#include <tf/transform_listener.h>
#include <std_msgs/Float64.h>
#include <cmath>
class Distance
{
  public:
  Distance()
  {
    path_sub_ = nh_.subscribe("/person_path",1, &Distance::callback, this);
    dist_pub_ = nh_.advertise<std_msgs::Float64>("relative_distance",10000,true);
    FOV_pub_ = nh_.advertise<std_msgs::Float64>("FOV_angle2",10000,true);
  }
  void callback(const nav_msgs::Path::ConstPtr& pathData)
  {
    seq = pathData ->header.seq;
    posestamped_ = pathData -> poses[seq];
    posestamped_.header.frame_id = "map";
    posestamped_.header.stamp = ros::Time(0);
    listener_.waitForTransform("person_detection_frame","map",ros::Time::now(),ros::Duration(3.0));
    listener_.transformPose("person_detection_frame", posestamped_,personpose_);
    float x = personpose_.pose.position.x;
    float z = personpose_.pose.position.z;
    distance.data = std::sqrt(std::pow(x,2) + std::pow(z,2));
    dist_pub_.publish(distance);
    FOV_angle.data  = -std::atan2(z,x) * 180 / 3.141592 +90;
    FOV_pub_.publish(FOV_angle);
}

private:
  ros::NodeHandle nh_;
  ros::Subscriber path_sub_;
  ros::Publisher dist_pub_;
  ros::Publisher FOV_pub_;

  tf::TransformBroadcaster br_;
  tf::Transform transform_;
  tf::TransformListener listener_;
  tf::StampedTransform stamped_;
  geometry_msgs::PoseStamped personpose_;
  geometry_msgs::PoseStamped posestamped_;
  geometry_msgs::PoseStamped global_stamp_;

  std_msgs::Float64 FOV_angle;
  std_msgs::Float64 distance;
  int seq;
};

int main(int argc, char **argv)
{
  ros::init(argc,argv,"distancenode");
  Distance tfobj;
  ros::spin();
  return 0;
}
