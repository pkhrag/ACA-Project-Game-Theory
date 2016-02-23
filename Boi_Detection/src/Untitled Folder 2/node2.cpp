#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
void call1Callback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Roger That");
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "node2");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("call1", 1000, call1Callback);
 
  ros::Publisher call2_pub = n.advertise<std_msgs::String>("call2", 1000);
    ros::spinOnce();

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {

    
    std_msgs::String msg;

    std::stringstream ss;
    ss << "Ready to take off" << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    call2_pub.publish(msg);


    loop_rate.sleep();
    ++count;
    
    ros::spinOnce();
  }
    
  return 0;
}
