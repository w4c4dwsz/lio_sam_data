#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <iostream>

/**
 * @brief add ring and times fileds to the poincloud2 msgs
 * 
 * @param inputLidarData 
 */
void lidarCallback(const sensor_msgs::PointCloud2 inputLidarData) {
    inputLidarData
}

int main(int argc, char** argv) 
{
    ros::init(argc, argv, "lio_sam_data_node");

    ros::NodeHandle nh;

    

    ros::Publisher  lioSamInputLidarData = nh.advertise<sensor_msgs::PointCloud2>("lidar_output", 100);

    ros::Subscriber lioSamOutPutLidatData = nh.subscribe<sensor_msgs::PointCloud2>("sf", 100, lidarCallback);
}