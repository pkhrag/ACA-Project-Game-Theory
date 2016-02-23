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

   //int camno=(**(argv+1)-'0');
   //VideoCapture cap(camno);
   VideoCapture cap(argv[1]);
   
   VideoWriter output_cap(argv[2],CV_FOURCC('D','I','V','X'),25,Size(640,480));

	 ros::init(argc, argv, "distance");
	 ros::NodeHandle n;

	ros::Publisher pub = n.advertise<std_msgs::Float32MultiArray>("balls", 1000);
	ros::Rate loop_rate(10);

 
  namedWindow("Orignal", CV_WINDOW_AUTOSIZE);
  namedWindow("Thresholded", CV_WINDOW_AUTOSIZE);
  namedWindow("imgthresh", CV_WINDOW_AUTOSIZE);
  int iLowH = 23;
  int iHighH = 98;

  int iLowS = 37; 
  int iHighS = 233;

  int iLowV = 104;
  int iHighV = 255;

	
  //Create trackbars in "Control" window
  createTrackbar("LowH", "Thresholded", &iLowH, 255); //Hue (0 - 179)
  createTrackbar("HighH", "Thresholded", &iHighH, 255);

  createTrackbar("LowS", "Thresholded", &iLowS, 255); //Saturation (0 - 255)
  createTrackbar("HighS", "Thresholded", &iHighS, 255);

  createTrackbar("LowV", "Thresholded", &iLowV, 255);//Value (0 - 255)
  createTrackbar("HighV", "Thresholded", &iHighV, 255);

  int iLastX = -1; 
  int iLastY = -1;
  vector<vector<Point> > contours;
  int count = 0,count_sec=0;
  int avg_radT=0,avg_rad=0;

  while(1)
  {
	    Mat imgOriginal,norimg,blur;
	    std_msgs::Float32MultiArray array;
	    ROS_INFO("1"); 
	    bool bSuccess = cap.read(imgOriginal); // read a new frame from video
      if (!bSuccess) //if not success, break loop
      {
          cout << "Cannot read a frame from video stream" << endl;
          break;
      } 
      Mat imgThresholded,img_hsv;
      vector<Mat> channels; 
      Mat img_hist_equalized,imgthresh;

      cvtColor(imgOriginal, img_hist_equalized, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format
      ROS_INFO("2"); 
      split(img_hist_equalized,channels); //split the image into channels
      equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)

      merge(channels,img_hist_equalized); //merge 3 channels including the modified 1st channel into one image

      cvtColor(img_hist_equalized, img_hist_equalized, CV_YCrCb2BGR); //change the color image from YCrCb to BGR format (to display image properly)
      GaussianBlur(img_hist_equalized,blur,Size(3,3),0,0,BORDER_DEFAULT);
      cvtColor(img_hist_equalized,img_hsv,CV_RGB2HSV);
      inRange(img_hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image 
      //morphological opening (removes small objects from the foreground)
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

      //morphological closing (removes small holes from the foreground)
      dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
      erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
      imgthresh=imgThresholded;
      ROS_INFO("3"); 
      imshow("Orignal",imgOriginal);
      imshow("imgthresh",imgthresh);
      resize(imgOriginal,imgOriginal,Size(640,480));
      output_cap.write(imgOriginal);
      
      resize(imgOriginal,imgOriginal,Size(imgOriginal.cols,imgOriginal.rows)); 

      vector<Point2f>center(1);
      vector<float>radius(1);
 
      findContours( imgThresholded, contours,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
      double largest_area,largest_contour_index;
         
      if(contours.empty()&&contours.size()>10) continue;
      ROS_INFO("4"); 
      //ROS_INFO("%f",contours.size); 
      for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
      {
              printf("%d\n",contours.size() );

         double a=contourArea( contours[i],false);  //  Find the area of contour
         if(a>largest_area)
         {
           largest_area=a;
           largest_contour_index=i;                //Store the index of largest contour
         }
       }  
   
      ROS_INFO("5"); 
      minEnclosingCircle(contours[largest_contour_index],center[0],radius[0]);
      ROS_INFO("7"); 
      //show the image in which identified shapes are marked   

      int key=waitKey(30);
      if (key== 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
      {
            cout << "esc key is pressed by user" << endl;
            break; 
      }
      Point2f pt;
      pt.x=imgOriginal.cols/2;
      pt.y=imgOriginal.rows/2;
      circle(imgThresholded,center[0],radius[0],Scalar(150,150,150),1,8,0);
      circle(imgThresholded,pt,1,Scalar(150,150,150),1,8,0);
      ROS_INFO("7"); 
      
      ROS_INFO("8");
      array.data.clear();
      if((count<10)&&(((radius[0]-avg_rad)<5)&&(-5<(radius[0]-avg_rad))))
      {
        count++;
        avg_radT+=radius[0];
        avg_rad=avg_radT/count;
        array.data.push_back(count);
        ROS_INFO("aaa");
        loop_rate.sleep();
      } 
      else if((count==10)&&(((radius[0]-avg_rad)<5)&&(-5<(radius[0]-avg_rad))))
      {
         float distance;
         avg_rad=avg_radT/10;
         array.data.clear();
         cin >> distance; 
         array.data.push_back(avg_rad);
         array.data.push_back(radius[0]);
         array.data.push_back((imgOriginal.cols/2-center[0].x));
         array.data.push_back((imgOriginal.rows/2-center[0].y));
         array.data.push_back(distance);

         pub.publish(array);
      
         imshow("Thresholded",imgThresholded);
         ros::spinOnce();
         loop_rate.sleep();
         avg_rad=radius[0];
         avg_radT=0;
         count=0;
         ROS_INFO("zzzz"); 
         continue;
      } 
      else
      {

        
        array.data.push_back(avg_rad);
        array.data.push_back(radius[0]);
        array.data.push_back((imgOriginal.cols/2-center[0].x));
        array.data.push_back((imgOriginal.rows/2-center[0].y));
        

        pub.publish(array);

        imshow("Thresholded",imgThresholded);
        ros::spinOnce();
        count=0;
        avg_rad=radius[0]; 
        avg_radT=0;   
        loop_rate.sleep();
        
      }
      array.data.push_back(avg_rad);
      array.data.push_back(radius[0]);
      array.data.push_back((imgOriginal.cols/2-center[0].x));
      array.data.push_back((imgOriginal.rows/2-center[0].y));
      imshow("Thresholded",imgThresholded);
      ros::spinOnce();
      loop_rate.sleep();

  }
   output_cap.release(); 
   return 0;
}

