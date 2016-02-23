#include <cv.h>
#include <highgui.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"

#include "std_msgs/Float32MultiArray.h"

#include <sstream>
using namespace std;
using namespace cv;

int main(int argc,char** argv)
{

   int camno=(**(argv+1)-'0');
   VideoCapture cap(camno);
   //VideoCapture cap(argv[1]);
   //VideoCapture cap(argv[1]-'0'); 
  
   
   VideoWriter output_cap(argv[2],CV_FOURCC('D','I','V','X'),25,Size(640,480));

	 ros::init(argc, argv, "distance");
	 ros::NodeHandle n;

	ros::Publisher pub = n.advertise<std_msgs::Float32MultiArray>("balls", 1000);
	ros::Rate loop_rate(10);
	int count = 0;
 
  namedWindow("Orignal", CV_WINDOW_AUTOSIZE);
  namedWindow("Thresholded", CV_WINDOW_AUTOSIZE);
  int iLowH = 105;
  int iHighH = 255;

  int iLowS = 0; 
  int iHighS = 140;

  int iLowV = 139;
  int iHighV = 255;

	
  //Create trackbars in "Control" window
  createTrackbar("LowH", "Thresholded", &iLowH, 255); //Hue (0 - 179)
  createTrackbar("HighH", "Thresholded", &iHighH, 255);

  createTrackbar("LowS", "Thresholded", &iLowS, 255); //Saturation (0 - 255)
  createTrackbar("HighS", "Thresholded", &iHighS, 255);

  createTrackbar("LowV", "Thresholded", &iLowV, 255);//Value (0 - 255)
  createTrackbar("HighV", "Thresholded", &iHighV, 255);

  vector<vector<Point> > contours;

  while(1)
  {
	    Mat imgOriginal;
	    std_msgs::Float32MultiArray array;
	  
	    bool bSuccess = cap.read(imgOriginal); // read a new frame from video
      if (!bSuccess) //if not success, break loop
      {
          cout << "Cannot read a frame from video stream" << endl;
          break;
      } 
      Mat imgThresholded;

      inRange(imgOriginal, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image 
      //morphological opening (removes small objects from the foreground)
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

      //morphological closing (removes small holes from the foreground)
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      ROS_INFO("1");
      imshow("Orignal",imgOriginal);
      resize(imgOriginal,imgOriginal,Size(640,480));
      output_cap.write(imgOriginal);
      
      resize(imgOriginal,imgOriginal,Size(imgOriginal.cols,imgOriginal.rows)); 

      vector<Point2f>center(1);
      vector<float>radius(1);
      ROS_INFO("2");
 
      /*findContours( imgThresholded, contours,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
      double largest_area,largest_contour_index;
         
      if(contours.empty()) continue;
       
      for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
      {
         double a=contourArea( contours[i],false);  //  Find the area of contour
         if(a>largest_area)
         {
           largest_area=a;
           largest_contour_index=i;                //Store the index of largest contour
         }
       }  
   
      
      minEnclosingCircle(contours[largest_contour_index],center[0],radius[0]);
      //show the image in which identified shapes are marked   
*/
      int key=waitKey(30);
      if (key== 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
      {
            cout << "esc key is pressed by user" << endl;
            break; 
      }
     /* Point2f pt;
      pt.x=imgOriginal.cols/2;
      pt.y=imgOriginal.rows/2;
      circle(imgThresholded,center[0],radius[0],Scalar(150,150,150),1,8,0);
      circle(imgThresholded,pt,1,Scalar(150,150,150),1,8,0);
 */
      array.data.clear();
   /*   array.data.push_back(radius[0]);
      array.data.push_back((imgOriginal.cols/2-center[0].x));
      array.data.push_back((imgOriginal.rows/2-center[0].y));

      pub.publish(array);
    */   
      imshow("Thresholded",imgThresholded);
      ros::spinOnce();
      ROS_INFO("3");
      loop_rate.sleep();
  }
  ROS_INFO("3");
 output_cap.release(); 
 return 0;
}
