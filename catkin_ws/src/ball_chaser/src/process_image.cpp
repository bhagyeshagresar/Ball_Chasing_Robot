#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;
  
  if (!client.call(srv)){
    ROS_ERROR("failed to call service");
  
  }

  
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img){
  int white_pixel = 255;
  int left_side = (img.step)/3;
  int right_side = (img.step) * 2/3;
  int pixel_position;
  
  for(int i = 0; i < img.height * img.step; i++){
    if (img.data[i] == white_pixel && img.data[i+1] == white_pixel && img.data[i+2] == white_pixel){
      
      pixel_position = i % img.step;
      break;
    }
    
  }
  
  if (pixel_position < left_side){
    ROS_INFO("Drive Left");
    drive_robot(0, 1.0);
    
  }
  
  else if (pixel_position > left_side && pixel_position < right_side){
    ROS_INFO("Drive Straight");
    drive_robot(1.0, 0);
  
  }
  
  else if (pixel_position > right_side){
    ROS_INFO("Drive Right");
    drive_robot(0, -1.0);
  }
  
  else if (pixel_position < 0){
    drive_robot(0, 0);
  }
  
  
  
}
  
  
  
  
      
      
    



int main(int argc, char** argv)
{
  // Initialize the process_image node and create a handle to it
  ros::init(argc, argv, "process_image");
  ros::NodeHandle n;
  
 // Define a client service capable of requesting services from command_robot
  client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
  
 // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
  ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);
  
 // Handle ROS communication events
  ros::spin();
  
  return 0;
  
}

  


  
  
  
  
