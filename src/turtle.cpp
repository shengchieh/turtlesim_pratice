#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"

#define PI 3.14159

ros::Publisher pub;
ros::Subscriber sub;

geometry_msgs::Twist vel;

int state = 0;


void Callback(const turtlesim::PoseConstPtr& msg)
{
	if(state != 7)
	{
		ROS_INFO("x: %.2f, y: %.2f, theta: %.2f", msg->x, msg->y, msg->theta);
	}
	else
	{
		ROS_INFO("Mission complete !");
	}

	if(state == 0)
	{
		if((msg->x) < 6.54)
			vel.linear.x = 0.5;
		else
		{
			vel.linear.x = 0;
			state = 1;
		}
	}

	if(state == 1)
	{
		if((msg->theta) < PI/2)
			vel.angular.z = 0.5;
		else
		{
			vel.angular.z = 0;
			state = 2;
		}
	}

	if(state == 2)
	{
		if((msg->y) < 6.54)
			vel.linear.x = 0.5;
		else
		{
			vel.linear.x = 0;
			state = 3;
		}
	}

	if(state == 3)
	{
		if((msg->theta) < PI)
			vel.angular.z = 0.5;
		else
		{
			vel.angular.z = 0;
			state = 4;
		}
	}

	if(state == 4)
	{
		if((msg->x) >= 5.54)
			vel.linear.x = 0.5;
		else
		{
			vel.linear.x = 0;
			state = 5;
		}
	}

	if(state == 5)
	{
		if((msg->theta) < 3*PI/2)
			vel.angular.z = 0.5;
		else
		{
			vel.angular.z = 0;
			state = 6;
		}
	}

	if(state == 6)
	{
		if((msg->y) >= 5.54)
			vel.linear.x = 0.5;
		else
		{
			vel.linear.x = 0;
			state = 7;
		}
	}

	pub.publish(vel);
}

int main(int argc, char **argv)
{
	// Initialize the node
	ros::init(argc, argv, "turtle");
	ros::NodeHandle node;

	// A publisher for the turtle movement
	pub = node.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

	// A subscriber for the turtle pose
	sub = node.subscribe("/turtle1/pose", 10, Callback);

	// Loop at 10Hz, publishing movement commands until we shut down
	ros::Rate rate(10);

	while (ros::ok()) 
	{
		ros::spinOnce();    // Allow processing of incoming messages

		rate.sleep();
	}
}

