#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Vector3.h"

#include <sstream>
#include <stdlib.h>

ros::Publisher p;

geometry_msgs::Twist desVel;
geometry_msgs::Twist newTwist;
std::vector<float> laserRanges;
float rangeMin;
const float rangeThreshold = 0.5;

void laserFeedback( const sensor_msgs :: LaserScan :: ConstPtr & msg){
	laserRanges = msg->ranges;
	rangeMin=laserRanges[0];
	for(int i=0;i<laserRanges.size();i++){
		if(laserRanges[i]<rangeMin){
			rangeMin=laserRanges[i];
		}
	}
	ROS_INFO("MIN: %f",rangeMin);
}

void calcNewVel( const geometry_msgs :: Twist :: ConstPtr & msg){
	desVel.linear = msg->linear;
	desVel.angular = msg->angular;

	if(desVel.linear.x>=0.01){
		ROS_INFO("FORWARD MOTION");
	}
	else{
		ROS_INFO("NO FORWARD");
	}

	geometry_msgs::Vector3 ZeroVec;

	ZeroVec.x = 0.0;
	ZeroVec.y = 0.0;
	ZeroVec.z = 0.0;

	geometry_msgs::Twist newTwist;

	if(rangeMin<=rangeThreshold && desVel.linear.x>=0.05){
		newTwist.linear = ZeroVec;
		ROS_INFO("OBSTACLE FOUND IN PATH");
	}
	else{
		newTwist.linear = desVel.linear;
		ROS_INFO("NO OBSTACLE");
	}

	newTwist.angular = desVel.angular;

	ROS_INFO("DES_VEL:[%f,%f,%f]",desVel.linear.x,desVel.linear.y,desVel.linear.z);
	ROS_INFO("NEW_VEL:[%f,%f,%f]",newTwist.linear.x,newTwist.linear.y,newTwist.linear.z);

	p.publish(newTwist);

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "teleop_node");

	// Process command line parameter looking for a -n string name
	// and should be placed after the ros::init() invocation.
	// rosrun <package_name> <executable_name> -n <new_name>
	// or
	// rosrun subscriber_package subscriber_node -n alternate_topic
	int opt;
	char* topic_name;
	(opt = getopt(argc, (argv), "n:"));
	if (opt != -1) {
	  switch (opt) {
	    case 'n':
	      topic_name = optarg;
	      ROS_INFO("%s",optarg);
	      ROS_INFO("%s",topic_name);
	      break;
	    default:
	      printf("The -%c is not a recognized parameter\n", opt);
	      topic_name = "des_vel";
	      ROS_INFO("%s",topic_name);
	      break; 
	  }
	}
	else{
		topic_name = "des_vel";
	    ROS_INFO("%s",topic_name);
	}
	ROS_INFO("%s", topic_name);
	ros::NodeHandle n;
	p = n.advertise<geometry_msgs::Twist>("robot0/cmd_vel",1000);
	ros::Subscriber s = n.subscribe(topic_name,1000,calcNewVel);
	ros::Subscriber lidarSub = n.subscribe("robot0/laser_1",1000,laserFeedback);
	ros::Rate loop_rate(1);
	while(ros::ok())
	{
		//lidarSub = n.subscribe("robot0/laser_1",1000,laserFeedback);
		//s = n.subscribe(topic_name,1000,calcNewVel);
		//ROS_INFO("DES_VEL:[%f,%f,%f]",desVel.linear.x,desVel.linear.y,desVel.linear.z);
		//ROS_INFO("NEW_VEL:[%f,%f,%f]",newTwist.linear.x,newTwist.linear.y,newTwist.linear.z);
		//p.publish(newTwist);
		ros::spinOnce();

	}
}