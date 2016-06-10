
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <queue>
#include <time.h>
#include <stdarg.h>
#include <assert.h>
#include <ostream>


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;




/*-------------------------------------------------------- Structure Declarations--------------------------------------------------------*/




typedef struct imagestructure
{
	int x;
	int y;
	int label;
	int value;
	int mapped;

}is;


typedef struct connected_component
{
	struct imagestructure im;
	struct connected_component *nxt;
}cc;



typedef struct numberofcc
{
	struct connected_component *start;
	struct connected_component *last;
	float gray_hist[256];
	/*int text_nontext
	 * @value : 0 if text
	 *          1 if non_text
	 */
	int text_nontext;
	/*int label
	 * @value : value of the color label from k-mean
	 */
	int label;
	int number_of_component;
	int flag;
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	int centroidx;
	int centroidy;
	int blk_no;
	int valid_blk_no;
	float histogram[256];
	double mean;
	double var;
	double std_dev;
	double skew;
	void calc_centroid()
	{
	  centroidx =  xmin+((xmax-xmin)/2);
	  centroidy =  ymin+((ymax-ymin)/2);
	}
	void calc_hist()
	{
	  
	  for(int i=0;i<256;i++)
	    histogram[i]=gray_hist[i]/number_of_component;
	}
	
	void calc_mean()
	{
	  mean = 0;
	  for(int i=0;i<256;i++)
	  {
	    mean = mean + (gray_hist[i]*i);
	  }
	  mean = mean/number_of_component;
	}
	
	
	void calc_dev()
	{
	  calc_mean();
	  int temp;
	  std_dev = 0.0;
	  var = 0.0;
	  skew = 0.0;
	  for(int i=0;i<256;i++)
	  {
	    for(int j=0;j<gray_hist[i];j++)
	    {
	      temp = i - mean;
	      var = var + (temp * temp);
	      skew = skew + (temp * temp *temp);
	    }
	  }
	  
	  var = var / number_of_component;
	  std_dev = sqrt(var);
	  
	}
	
	void calc_distribution()
	{
	  calc_hist();
	  calc_mean();
	  calc_dev();
	}
	
}nocc;


/*-----------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------- Global variables----------------------------------------------------------------------*/



extern Mat src;
extern int no_of_foregrnd_pix;
extern RNG rng;

extern vector<nocc> newparagraph;
extern int npar;



// parameters for binarization

extern int binary_threshold_value;


	 
/*-------------------------------------------------------------------------------------------------------------------------------------------*/



namespace IITkgp_functions {
  

class PixelValidation
{
public:
    /**
     * @function validate
     * @param : input param: co-ordinate position(i,j) and maximum-limit(t) row, col
     * @brief : co-ordinate position(i,j) to be check whether it is within given row and col
     * @return : 1 if it belong to particular region
     *           0 if not belong within that particular row and col
     */
    int validate(int i, int j, int row, int col);
};

class CC_analysis
{
 public:
    /**
     * @function labelingdfs
     * @param : input param: input image structure(of (IS(datatype))) input co-ordinate(m,n) to belabeled and image dimention(row,col)
     * @brief : labeling based on DFS algorithm
     * @return : image structure(of (IS(datatype)))
     */
    is ** labelingdfs(is **tempstructuredimage,int *m, int *n, int temp_row, int temp_col);

    /**
     * @function cclabeling
     * @param : input param: input image structure(of (IS(datatype))) and image dimention(row,col)
     * @brief : labeling of ConnectedComponent based on DFS algorithm
     * @return : image structure(of (IS(datatype)))
     */


    is** cclabeling(is **tempstructuredimage,int temp_row, int temp_col);

    /**
     * @function FindConnectedComponent
     * @param : input param: input image(binary) Mat data-type
     *                             src_gray(gray scale) Mat data-type
     * @brief : Find connected component from the input binary image
     * @return : array of connected components (nocc(data-type))
     */


    nocc* FindConnectedComponent(Mat image, Mat src_gray);
};

  
class BinaryImage
{
public:

        //void BinaryThreshold( int, void* );

        /**
         * @function binarization
         * @param input an image in Mat format and type for binarization
         * @brief type = 1 for adaptive
         * @brief type = 2 for otsu
         * @brief type = 3 for Normal Threshold by GUI
         * @brief type = 4 for Normal Threshold by fixed value
         * @return Return a binary image of Mat data-type
          */
        Mat binarization(Mat image, int type);


};

class ImageProcFunc
{
public:

    /**
     * @function foreground_masked_image
     * @param input a color image in Mat format and it's corresponding binary image
     * @brief convert an input image into a uniform background image
     * @brief masked the foreground pixels and make the background pixel uniform
     * @return Return a uniform background image of Mat data-type
     */
    Mat foreground_masked_image(Mat ColorImage, Mat binary);

    /**
     * @function NumberofForegroundPixel
     * @param input a binary image in Mat format
     * @brief It count number of foreground pixel in the given image
     * @return Return a integer which gives the count of number of foreground pixel
     */
    int NumberofForegroundPixel(Mat image);

    /**
     * @function FindImageInverse
     * @param input Mat data(image)
     * @brief Calculate inverse of a given image (255 - image.data[i])
     * @return inverse image(Mat)
     */
    Mat FindImageInverse(Mat image);

};


class Morphology
{
public:
    /**
     * @function erosion
     * @param input an image(binary) in Mat format
     * @brief it erode an image with a square mask of 3x3
     * @return Return eroded image of Mat data-type
     */

    Mat erosion(Mat image);


    /**
     * @function boundaryextraction
     * @param input an image(binary) in Mat format
     * @brief it find the boundary of the input image
     * @return Return boundary of input image(binary in nature)
     */

    Mat boundaryextraction(Mat image);

    /**  @function Erosion
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input erosion Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Eroded Image of the input image with given kernel type and size
     * @return Return Eroded image of input image
     */
    Mat Erosion( int erosion_elem, int erosion_size, Mat image);

    /**  @function Dilation
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input Dilation Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Dilated Image of the input image with given kernel type and size
     * @return Return Dilateded image of input image
     */
    Mat Dilation( int dilation_elem, int dilation_size, Mat image );

    /**  @function Open
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input element Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Open Image of the input image with given kernel type and size
     * @return Return Open image of input image
     */
    Mat Open(int open_elem, int open_size, Mat image);

    /**  @function Close
     * @param input
     * element type
     * 0: kernel = Rectangle
     * 1: kernel = CROSS
     * 2: kernel = ELLIPSE
     * @param input element Size(n) : Create a kernel or window of 2n+1
     * @param input an image in Mat format(image).
     * @brief it find Close Image of the input image with given kernel type and size
     * @return Return Close image of input image(binary in nature)
     */
    Mat Close(int close_elem, int close_size, Mat image);


    /**  @function Morphology_Gui
     * @param input
     *                 Mat image
     *                 int type
     *                  0: Erosion
     *                  1: Dilation
     *                  2: Open
     *                  3: Close
     *
     * @return Return Morphologycally processed image
     */

    Mat Morphology_Gui(Mat image, int type);

};

class SmoothingGapfilling
{
public:
    /**
     * @function horizontal_smoothing
     * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
     * @brief with this function from a foreground position , next whitespace number of pixel is filled in horizontal direction
     * @brief it produce smoothed image of the input image in horizontal direction by filling up with foreground with whitespace amount
     * @return Return horizontally smoothed image of input image(binary in nature)
     */

    Mat horizontal_smoothing(Mat image, int whitespace);

    /**
     * @function horizontal_gapfilling
     * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
    * @brief with this function gap btween two foreground pixel is filled only if the gap between two foreground pixel in horizontal direction has a gap less than or equal to whitespace
     * @brief it produce smoothed image of the input image in horizontal direction by filling up with foreground with whitespace amount
     * @return Return gap-filled image of input image(binary in nature)
     */

    Mat horizontal_gapfilling(Mat image, int whitespace);

    /**
     * @function vertical_smoothing
     * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
     * @brief with this function from a foreground position , next whitespace number of pixel is filled
     * @brief it produce smoothed image of the input image in vertical direction by filling up with foreground with whitespace amount
     * @return Return vertically smoothed image of input image(binary in nature)
     */

    Mat vertical_smoothing(Mat image,int whitespace);

    /**
     * @function vertical_gapfilling
     * @param input an image(binary) in Mat format and integer value whitespace that need to be gap filled or filled up
     * @brief with this function gap btween two foreground pixel is filled only if the gap between two foreground pixel in vertival direction has a gap less than or equal to whitespace
     * @brief it produce gap filled image of the input image in vertical direction by filling up with foreground with whitespace amount
     * @return Return vertically gap-filled image of input image(binary in nature)
     */


    Mat vertical_gapfilling(Mat image,int whitespace);



    Mat smoothinggapfilling_GUI(Mat image, int sgtype);

};


class RectangleTest
{
public:
    /**
     * @Function: PointRectangleTest
     * @brief : Take 1 rectangle and a Point as input
     * @brief : Test whether the Given Point is inside the Given Rectangle or Inside
     * @return : 	0: Point is Outside of Rectangle
     * 		1: Point is inside the given Rectangle
     * */

    int PointRectangleTest(Rect GivenRect, Point GivenPoint);

    /**
     * @Function: FindOverlappingRectangles
     * @brief : Take 2 rectangle as an input
     * @return : 	0: Rect 1 and Rect 2 are disjoint
     * 		1: Rect 1 is inside Rect 2
     * 	    	2: Rect 2 is inside Rect 1
     * 	    	3: Rect 1 and 2 are partly overlapped
     *
     *
     * */

    int FindOverlappingRectangles(Rect first, Rect second);
};



}


/**************************************************//**
 * \file xml.h
 * \author Ritu Garg
 * \dated 03-03-2010
 * \brief Defines the routines for parsing, writing and updating XML file using the xmlParser Library.  
 *****************************************************/
 /**
  * \author Soumyadeep Dey
  * \modified date 03-03-12
  * 
  */


namespace IITKGP_XML_IO
{
/**	\class XML_IO
*	\brief class contains all variables and function for reading and creating XML file
*/

class XML_IO
{
	public:
/**			\fn char* readXML(char *XMLFile)
 *			\brief Parses the input XML file structure and reads the input document page to be processed. 
 *			\param char *XMLFile stores the name of the input XML file
 *			\return input document pag to be processed.
 */
		char* readXML(char *XMLFile);
		
/**			\fn void WriteOutXML(char *inputXMLfile, char *outXML, char *tx_in_page, char *tx_out_page,
	   			 char *gr_in_page, char *gr_out_page, int graphFlag, vector<int>& blk_Label, 
				 vector<int>& x1,vector<int>& x2,vector<int>& y1, vector<int>& y2)
 *			\brief write the intemediate as well as final output of segmentation and classification into XML file
 *			\param char *inputXMLfile stores the input XML file
 *			\param char *outXML stores the output XML file name
 *			\param char *tx_in_page stores the name of the input page fpr text extraction
 *			\param char *tx_out_page stores the name of the final output after all the text has been extracted.
 *			\param char *gr_in_page stores the name of the page given as input for graphics recognition
 *			\param char *gr_out_page storesthe name of fial output of the graphics extracted from the input page
 *			\param int graphFlag indicates the presence of graphics blocks in the input page
 *			\param vector<int>& blk_Label stores the information as to which block is a text block or a graphics block
 *			\param vector<int>& x1,vector<int>& x2,vector<int>& y1, vector<int>& y2 store the top left and bottom right coordinates of the blocks generated after segmentation.
 *			\return none
 */
		void WriteOutXML(char *inputXMLfile, char *outXML, char *tx_in_page, char *tx_out_page,
	   			 char *gr_in_page, char *gr_out_page, int graphFlag, vector<int>& blk_Label, 
				 vector<int>& x1,vector<int>& x2,vector<int>& y1, vector<int>& y2);
/**			\var char *page
 *			\brief stores the input image name
*/
		char *page;

/**			\var int cropFlag
 *			\brief indicates whether COPPING tag exists inthe input XML file
*/
		int cropFlag;

/**			\var vector <int> leftX,leftY,rightX,rightY
 *			\brief stores top left and bottom right coordinates of the blocks obtained after segmentation
*/
		vector <int> leftX,leftY,rightX,rightY;
		
/**			 \fn void writemyxml(char *inputXML ,char *outputxml, char *input_image, char *output_image)
 * 			 \brief write the outputxml of marginal moise remove
 * 			 \param char *inputXML stores the input XML file
 * 			 \param char *outputxml stores the output XML file name
 *			 \param char *childtype give the type of child
 * 			 \param char *input_image stores the input image name
 * 			 \param char *output_image stores the output image name
 * 			 \param char *angel skew angel
 * 			 \param char *algorithmname name of the algorithm used for skew correction 
 * 			 \param int  toplx is the top-most left, x co-ordinate of the detected page frame
 * 			 \param int  toply is the top-most left, y co-ordinate of the detected page frame
 * 			 \param int  btmrx is the bottom-most right x-co-ordinate of the detected page frame
 * 			 \param int  btmry is the bottom-most right y-co-ordinate of the detected page frame
 * 
 */
		void writemyxml(char *inputXML ,char *outputxml, char *childtype,char *input_image, char *output_image,
				char *angel, char *algorithmname, int toplx, int toply, int btmrx, int btmry);

		XML_IO();///<the constructor.
		~XML_IO();///<the desttructor.
};


}



