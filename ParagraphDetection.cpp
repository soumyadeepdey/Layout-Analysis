
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~* 
 
 
#include "myheader.h"


vector<nocc> newparagraph;
int npar;
Mat src;
int binary_threshold_value;
RNG rng(12345);

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @function readme
 */
void readme()
{ std::cout << " Usage: ./para <input.xml> <output.xml> <output_image_name>" << std::endl; }


/*-------------------------------------------------MAIN--------------------------------------------------------------------------------------*/



int main(int argc, char *argv[])
{
  if( argc != 4 )
  { readme(); return -1; }

   char *input_image_name;
   input_image_name = (char *)malloc(4000*sizeof(char));

   IITKGP_XML_IO::XML_IO xml;
   
   input_image_name = xml.readXML(argv[1]);
   
   printf("\nInput image name %s\n",input_image_name);
  
   src=imread(input_image_name,1);
   
   IITkgp_functions::BinaryImage bin;
   
   binary_threshold_value = 120;
   Mat binary_dst;
   binary_dst = bin.binarization(src,4);
   //imwrite("BinaryImage.jpg",binary_dst);
   
   vector<vector<Point> > contours;
   vector<Vec4i> hierarchy;
  
    Mat temp_img;
    
    
    IITkgp_functions::ImageProcFunc imgproc;
    
    binary_dst.copyTo(temp_img);
    temp_img = imgproc.FindImageInverse(temp_img);
  
  /// Find contours
  findContours( temp_img, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
  
  
  /// Approximate contours to polygons + get bounding rects and circles
  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  


  
  for( int j = 0; j < contours.size(); j++ )
     { 
       approxPolyDP( Mat(contours[j]), contours_poly[j], 3, true );
       boundRect[j] = boundingRect( Mat(contours_poly[j]) );
     }
     
     
     /**
   * @Var initial_contour_flag
   * @brief size equal to number of original Contour from eroded image
   * @brief flag value is 1 if parent contour
   *        flag value is 0 if child contour
   *        flag value is 2 if contour are large_block(parent)
   *        flag value is 3 if contour ae small and suppose to be noise
   * */
  int *initial_contour_flag;
  initial_contour_flag = (int *)malloc(contours.size()*sizeof(int));
  for(int j = 0; j < contours.size(); j++ )
    initial_contour_flag[j]=1;
  
  Mat drawing;
  src.copyTo(drawing);
  int avg_height = 0;
  int number_of_block = 0;
  for( int j = 0; j< contours.size(); j++ )
  {
    if(hierarchy[j][3] == -1)
    {
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      rectangle( drawing, boundRect[j].tl(), boundRect[j].br(), color, 6, 8, 0 );
      Rect tmpr = boundRect[j];
      avg_height = avg_height +  tmpr.height;
      number_of_block = number_of_block + 1;
    } 
    else
      initial_contour_flag[j] = 0;
  }
  
  avg_height = avg_height/number_of_block;
  
  printf("Average Height = %d\n",avg_height);
  drawing.release();
  //imwrite(argv[3], drawing);
  
  IITkgp_functions::SmoothingGapfilling sg;
  
  Mat horizgapimage = sg.horizontal_gapfilling(binary_dst,avg_height*2);
  Mat verigapimage = sg.vertical_gapfilling(horizgapimage,avg_height*2);
  
  temp_img.release();
  verigapimage.copyTo(temp_img);
  verigapimage.release();
  temp_img = imgproc.FindImageInverse(temp_img);
  
  horizgapimage.release();
  
  contours.clear();
  hierarchy.clear();
  
   /// Find contours
  findContours( temp_img, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
  temp_img.release();
  
  contours_poly.clear();
  boundRect.clear();
  contours_poly.resize(contours.size());
  boundRect.resize(contours.size());
  
  
  for( int j = 0; j < contours.size(); j++ )
     { 
       approxPolyDP( Mat(contours[j]), contours_poly[j], 3, true );
       boundRect[j] = boundingRect( Mat(contours_poly[j]) );
     }
     
     

  
  
  src.copyTo(drawing);
  
  int pageframexmax = 0;
  int pageframeymax = 0;
  int pageframexmin = src.cols;
  int pageframeymin = src.rows;

  for( int j = 0; j< contours.size(); j++ )
  {
    if(hierarchy[j][3] == -1)
    {
      //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
     // rectangle( drawing, boundRect[j].tl(), boundRect[j].br(), color, 6, 8, 0 );
      //page frame
      nocc tempcc;
      tempcc.xmin = boundRect[j].x;
      tempcc.ymin = boundRect[j].y;
      tempcc.xmax = boundRect[j].x+boundRect[j].width;
      tempcc.ymax = boundRect[j].y+boundRect[j].height;
      newparagraph.push_back(tempcc);

      if(pageframexmin > boundRect[j].x)
	pageframexmin = boundRect[j].x;
      if(pageframeymin > boundRect[j].y)
	pageframeymin = boundRect[j].y;
      if(pageframexmax < boundRect[j].x+boundRect[j].width)
	pageframexmax = boundRect[j].x+boundRect[j].width;
      if(pageframeymax < boundRect[j].y+boundRect[j].height)
	pageframeymax = boundRect[j].y+boundRect[j].height;

    } 

  }

  npar = newparagraph.size();
 // rectangle( drawing, Point(pageframexmin,pageframeymin), Point(pageframexmax,pageframeymax), Scalar(0,0,255), 6, 8, 0 );
  
  imwrite(argv[3], drawing);
  drawing.release();
     
  
  char *xmlchild;
	xmlchild = "TextBlock";
	xml.writemyxml(argv[1],argv[2],xmlchild,input_image_name,argv[3],"NULL", "NULL", pageframexmin, pageframeymin, pageframexmax, pageframeymax);

  printf("\n");
  return 0;
}
