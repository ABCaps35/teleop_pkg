#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"

#include <sstream>

void calcNewVel(){

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "publisher_node");
	ros::NodeHandle n;
	ros::Publisher p = n.advertise<geometry_msgs::Twist>("topic_name",1000);
	ros::Subscriber s = n.subscribe("topic_name",1000,calcNewVel);
	ros::Rate loop_rate(1);
	int newVel=0;
	while(ros::ok())
	{
		p.publish(newVel);
		ros::spinOnce();
	}
}