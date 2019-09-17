#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"

#include <sstream>
#include <stdlib.h>

void calcNewVel(){

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "publisher_node");
	// Process command line parameter looking for a -n string name
	// and should be placed after the ros::init() invocation.
	// rosrun <package_name> <executable_name> -n <new_name>
	// or
	// rosrun subscriber_package subscriber_node -n alternate_topic
	int opt;
	while ((opt = getopt(argc, (argv), "n:")) != -1) {
	  switch (opt) {
	    case 'n':
	      topic_name = optarg;
	      break;
	    default:
	      printf("The -%c is not a recognized parameter\n", opt);
	      break; 
	  }
	}
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