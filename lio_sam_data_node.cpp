#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <iostream>

int main(int argc, char** argv) 
{
    ros::init(argc, argv, "lio_sam_data_node");

    ros::NodeHandle nh;

    ros::Publisher  lioSamInputLidarData = nh.advertise
}