#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <std_msgs/Float64.h>

ros::Publisher reference_height_pub;
std_msgs::Float64 reference_height_msg;

void objectPositionCallback(const geometry_msgs::PointStamped::ConstPtr& msg)
{
    // Extract the z value from the received message
    double z = msg->point.z;

    // Publish the z value to the reference_height topic
    
    reference_height_msg.data = z-50;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "height_publisher");
    ros::NodeHandle nh;

    reference_height_pub = nh.advertise<std_msgs::Float64>("/reference_height", 10);
    ros::Subscriber object_position_sub = nh.subscribe("/required_object_position/message", 10, objectPositionCallback);
    
    ros::Rate rate(10);  // Specify the publishing rate (10 Hz in this example)

    while (ros::ok())
    {
        ros::spinOnce();
        reference_height_pub.publish(reference_height_msg);
        rate.sleep();
    }

    return 0;
}
