#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;
bool white_found;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    //ROS_INFO("Drive bot function called");
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv)) {
	ROS_ERROR("failed to call service");	
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    white_found = false;
    //ROS_INFO("Starting for-loop");
    //ROS_INFO("size of image %1.2f height %1.2f step", (float)img.height, (float)img.step);

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
	for (int i = 0; i < img.height * img.step; i++) {
		//ROS_INFO("Pixel value: %1.2f, pixel number %1.2f", (float)img.data[i], (float)i);
		if (img.data[i] == white_pixel) {
			int row_ = i % img.step; 				
			float column_ = (float)row_ / ((float)img.step / 3.0);
			white_found = true;

			//ROS_INFO("row_ %i and column %1.2f", (int)row_, (float)column_); 		
			if (column_ < 1) {
				//ROS_INFO("turn left called");
				drive_robot(0.0, 0.5);
				break;
			}
			if (column_ > 2) {
				//ROS_INFO("turn right called");
				drive_robot(0.0, -0.5);
				break;
			} else {
				//ROS_INFO("drive forward called");
				drive_robot(0.5, 0.0);
				break;
			}
		}
    	}
	if (!white_found) {
		drive_robot(0.0, 0.0);
	}
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    //ROS_INFO("node initialized");

    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    //ROS_INFO("client defined");
    //drive_robot(0.0, 0.0);

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);
    //ROS_INFO("subscribed to the image_raw topic");

    // Handle ROS communication events
    //ROS_INFO("ros spin started");
    ros::spin();

    return 0;
}
