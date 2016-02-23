#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
  int i=0;
void call2Callback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Aye Aye Commander");
  i++;
  
}
int main(int argc, char **argv)
{

  ros::init(argc, argv, "node1");


  ros::NodeHandle n;

  ros::Publisher call1_pub = n.advertise<std_msgs::String>("call1", 1000);
  ros::Subscriber sub = n.subscribe("call2", 1000, call2Callback);

  ros::Rate loop_rate(10);

  int count = 0;
  do
  {
    std_msgs::String msg;

    std::stringstream ss;
    ss << "calling n1 to n2,do you copy" << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    call1_pub.publish(msg);

    loop_rate.sleep();
    ++count;
    while(i==0){
      ros::spinOnce();
  }
    ros::spinOnce();
 
  }while (ros::ok()&&i);
    


  return 0;
}
