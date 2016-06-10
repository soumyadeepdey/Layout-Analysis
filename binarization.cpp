

//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

#include "myheader.h"

/*------------------------------------------------------BINARIZATION-------------------------------------------------------------------*/



  /**
   * @param :thereshold_type
   *
   * 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */
int threshold_type = 0;
int const maximum_value = 255;
int const maximum_type = 4;
int const maximum_BINARY_value = 255;
int const blockSize= 101;
//int const blockSize=101;
Mat TempGray,TempBinary;



void BinaryThreshold( int, void* )
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */

  threshold( TempGray, TempBinary,  binary_threshold_value, maximum_BINARY_value,threshold_type );
  imshow("BinaryThresholding",TempBinary);
}


/**
 * @function binarization
 * @param input an image in Mat format and type for binarization
 * @brief type = 1 for adaptive
 * @brief type = 2 for otsu
 * @brief type = 3 for Normal Threshold by GUI
 * @brief type = 4 for Normal Threshold by fixed value
 * @return Return a binary image of Mat data-type
  */

Mat IITkgp_functions::BinaryImage::binarization(Mat image, int type)
{
/**
 * @param type
 * type = 1 for adaptive;
 * type = 2 for Otsu
 * type = 3 for Normal Threshold by GUI
**/

    // Convert the image to Gray

    Mat gray,binary;
    threshold_type = 0;

    cvtColor(image, gray, CV_BGR2GRAY);

    if(type == 1)
      adaptiveThreshold(  gray, binary, maximum_BINARY_value, ADAPTIVE_THRESH_GAUSSIAN_C,  threshold_type,  blockSize, 20);

    // Otsu Thresholding
    if(type == 2)
    {
      double val = threshold( gray, binary, 100, maximum_BINARY_value, cv::THRESH_OTSU | cv::THRESH_BINARY);
      printf("threshold value is %lf\n",val);
    }

    //GUI Threshold
    if(type == 3)
    {
      gray.copyTo(TempGray);
      /// Create a window to display results
        namedWindow( "BinaryThresholding", CV_WINDOW_KEEPRATIO );

        createTrackbar( "Value", "BinaryThresholding", & binary_threshold_value, maximum_BINARY_value, BinaryThreshold );

        /// Call the function to initialize
        BinaryThreshold( 0, 0 );
        waitKey(0);
        printf("threshold value is %d\n",binary_threshold_value);
        TempBinary.copyTo(binary);
    }

    // Fixed Threshold
    if(type == 4)
    {
      binary_threshold_value = 120;
      threshold( gray, binary,  binary_threshold_value, maximum_BINARY_value,threshold_type );
    }

    return (binary);

}



/*------------------------------------------------------------------------------------------------------------------------------------------------*/
