#include <ros/ros.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_cloud.h>
#include <pcl/io/io.h>
#include <pcl/filters/filter.h>
#include <iostream>
#include <string>



/**
 * define a custom point type
 */
struct PointXYZIRT
{
    PCL_ADD_POINT4D
    PCL_ADD_INTENSITY;
    uint16_t ring;
    float time;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} EIGEN_ALIGN16;

POINT_CLOUD_REGISTER_POINT_STRUCT (PointXYZIRT,
                                   (float, x, x) (float, y, y) (float, z, z) (float, intensity, intensity)
                                           (uint16_t, ring, ring) (float, time, time)
)
class node{
public:
    node()
    {
        lioSamInputLidarData = nh.advertise<sensor_msgs::PointCloud2>("points_raw", 100);
        lioSamOutPutLidarData = nh.subscribe<sensor_msgs::PointCloud2>("/lslidar_point_cloud", 100, &node::lidarCallback, this);
        ROS_INFO("this is node class make success");
    }

    /**
     * @brief add ring and times fileds to the poincloud2 msgs
     *
     * @param inputLidarData
     */
    void lidarCallback(const sensor_msgs::PointCloud2 inputLidarData)
    {
        ROS_INFO("enter lidarCallbac\n");
//    从msgs中提取出点云数据
        pcl::PointCloud<pcl::PointXYZI>::Ptr inputCloud(new pcl::PointCloud<pcl::PointXYZI>);
        pcl::PointCloud<PointXYZIRT>::Ptr outputCloud(new pcl::PointCloud<PointXYZIRT>);
        pcl::fromROSMsg(inputLidarData, *inputCloud);
        std::vector<int> indices;
        pcl::removeNaNFromPointCloud(*inputCloud, *inputCloud, indices);
//    产生新的PointCLoud2信息
        sensor_msgs::PointCloud2 outputMsg;

//    TODO: for every point in pointcloud, computer the ring and times

        for(auto point : inputCloud->points) {
            float verticalAngle = atan2(point.z, sqrt(point.x * point.x + point.y * point.y)) * 180 / M_PI;
            int rowIndex = (verticalAngle + ang_bottom) / ang_res_y;
            float threat = atan2(point.y, point.x) * 180.0 / M_PI;
            if(threat < 0)
                threat += 360.0;
//        TODO: how to calculate columnIndex, maybe get the data from lidar by self
//            int columnIndex = -round((threat - 90.0) / ang_res_x) + Horizon_SCAN / 2;
            int columnIndex = round(threat / ang_res_x);
            float times = (columnIndex / static_cast<float>(Horizon_SCAN)) * 0.1;

            printf("[x, y, z, rowIndex, threat, columnIndex, times] = [%f, %f, %f, %d, %f, %d, %f]\n",
                   point.x, point.y, point.z, rowIndex, threat, columnIndex, times);

            PointXYZIRT outPoint;
            outPoint.x = point.x;
            outPoint.y = point.y;
            outPoint.z = point.z;
            outPoint.ring = rowIndex;
            outPoint.time = times;
            outputCloud->push_back(outPoint);
        }
        pcl::toROSMsg(*outputCloud, outputMsg);
        ROS_INFO("enter callback");
        lioSamInputLidarData.publish(outputMsg);

    }

private:
    ros::NodeHandle nh;
    ros::Publisher lioSamInputLidarData;
    ros::Subscriber  lioSamOutPutLidarData;
    const int N_SCAN = 16;
    const int Horizon_SCAN = 1800;
    const float ang_res_x = 0.2;
    const float ang_res_y = 2.0;
    const float ang_bottom = 15.0+0.1;
    const int groundScanInd = 7;
};


int main(int argc, char **argv)
{
    ROS_INFO("this");
    ros::init(argc, argv, "lio_sam_data_node");
    node n;
    ROS_INFO("INIT SUCESS\n");
    while (ros::ok())
    {
        ros::spinOnce();
    }

    return 0;
    
}