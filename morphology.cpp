

//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*
// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*


#include "myheader.h"

/*----------------------------------------------MORPHOLOGICAL OPERATIONS----------------------------------------------------------------------*/



int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int morph_size = 0;
int morph_elem = 0;
int const max_elem = 2;
int const max_kernel_size = 21;


/*-------------------------------------------------------EROTION WITH 4 NEIGHBOURHOOD-------------------------------------------------------------*/


/**
 * @function erosion
 * @param input an image(binary) in Mat format
 * @brief it erode an image with a square mask of 3x3
 * @return Return eroded image of Mat data-type
 */

Mat IITkgp_functions::Morphology::erosion(Mat image)
{
    int row = image.rows;
    int col = image.cols;
    int i,j;
    Mat tempimage;
    image.copyTo(tempimage);
    for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
        tempimage.data[i*col+j] = 255;
    }

    for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            if(image.data[i*col+j]==0)
            {
                if(i-1<0||i+1>=row||j-1<0||j+1>=col)
                    tempimage.data[i*col+j]=255;
                else if(image.data[(i-1)*col+j]==0&&image.data[(i+1)*col+j]==0&&image.data[i*col+(j-1)]==0&&image.data[i*col+(j+1)]==0)
                    tempimage.data[i*col+j]=0;
                else
                    tempimage.data[i*col+j]=255;
            }
            else
                tempimage.data[i*col+j]=255;
        }
    }

    return (tempimage);


}

/*-------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------- BOUNDARY EXTRACTION--------------------------------------------------*/


/**
 * @function boundaryextraction
 * @param input an image(binary) in Mat format
 * @brief it find the boundary of the input image
 * @return Return boundary of input image(binary in nature)
 */

Mat IITkgp_functions::Morphology::boundaryextraction(Mat image)
{

    int i,j;

    Mat erodedimage;
    Mat extractedimage;

    image.copyTo(erodedimage);
    image.copyTo(extractedimage);
    int row,col;

    row = image.rows;
    col = image.cols;

    for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
      {
        erodedimage.data[i*col+j] = 255;
        extractedimage.data[i*col+j] = 255;
      }
    }

    erodedimage=erosion(image);

    for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            if(image.data[i*col+j]==erodedimage.data[i*col+j])
                extractedimage.data[i*col+j]=255;
            else
                extractedimage.data[i*col+j]=0;
        }
    }

    return(extractedimage);


}




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
Mat IITkgp_functions::Morphology::Erosion( int erosion_elem, int erosion_size, Mat image)
{
  int erosion_type;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
  Mat ErodedImage;
  /// Apply the erosion operation
  erode( image, ErodedImage, element );
  return(ErodedImage);

}


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
Mat IITkgp_functions::Morphology::Dilation( int dilation_elem, int dilation_size, Mat image )
{
  Mat DilatedImage;
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( image, DilatedImage, element );
  return(DilatedImage);

}


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
Mat IITkgp_functions::Morphology::Open(int open_elem, int open_size, Mat image)
{
  Mat OpenImage;

  int open_type;
  if( open_elem == 0 ){ open_type = MORPH_RECT; }
  else if( open_elem == 1 ){ open_type = MORPH_CROSS; }
  else if( open_elem == 2) { open_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( open_type,
                                       Size( 2*open_size + 1, 2*open_size+1 ),
                                       Point( open_size, open_size ) );
  Mat ErodedImage;
  erode(image, ErodedImage, element);
  dilate(ErodedImage, OpenImage, element);
  //ErodedImage = Erosion(open_elem,open_size, image);
  //OpenImage = Dilation(open_elem,open_size, ErodedImage);
  return(OpenImage);
}


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
Mat IITkgp_functions::Morphology::Close(int close_elem, int close_size, Mat image)
{
  Mat CloseImage;
  int close_type;
  if( close_elem == 0 ){ close_type = MORPH_RECT; }
  else if( close_elem == 1 ){ close_type = MORPH_CROSS; }
  else if( close_elem == 2) { close_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( close_type,
                                       Size( 2*close_size + 1, 2*close_size+1 ),
                                       Point( close_size, close_size ) );
  Mat DilatedImage;
  dilate( image, DilatedImage, element );
  erode(DilatedImage, CloseImage, element);
 // DilatedImage = Dilation(close_elem, close_size, image);
 // CloseImage = Erosion(close_elem, close_size, DilatedImage);
  return(CloseImage);
}


/*-------------------------------------------------------------------------------------------------------------------------*/

int Morphology_type;
Mat TempMorphImage,TempTargetImage;

void MorphOperation(int, void*)
{
    int morph_type;
      if( morph_elem == 0 ){ morph_type = MORPH_RECT; }
      else if( morph_elem == 1 ){ morph_type = MORPH_CROSS; }
      else if( morph_elem == 2) { morph_type = MORPH_ELLIPSE; }
    Mat element = getStructuringElement( morph_type,
                                         Size( 2*morph_size + 1, 2*morph_size+1 ),
                                         Point( morph_size, morph_size ) );
    if(Morphology_type == 0) // erosion
    {
        erode( TempTargetImage, TempMorphImage, element );
    }
    else if(Morphology_type == 1) // dilation
    {
        dilate(TempTargetImage, TempMorphImage, element);
    }
    else if(Morphology_type == 2) //open
    {
        Mat ErodedImage;
        erode( TempTargetImage, ErodedImage, element );
        dilate(ErodedImage, TempMorphImage, element);
        ErodedImage.release();
    }
    else if(Morphology_type == 3) //close
    {
        Mat DilatedImage;
        dilate( TempTargetImage, DilatedImage, element );
        erode(DilatedImage, TempMorphImage, element);
        DilatedImage.release();
    }
    imshow("Morphological Operartion",TempMorphImage);
}

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

Mat IITkgp_functions::Morphology::Morphology_Gui(Mat image, int type)
{
    Mat ProcessedImage;
    Morphology_type = type;
    image.copyTo(TempTargetImage);

    namedWindow("Morphological Operartion",CV_WINDOW_KEEPRATIO);

    createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Morphological Operartion",
                      &morph_elem, max_elem,
                      MorphOperation );

    createTrackbar( "Kernel size:\n 2n +1", "Morphological Operartion",
                      &morph_size, max_kernel_size,
                      MorphOperation );

    MorphOperation(0,0);
    waitKey(0);
    TempTargetImage.release();
    image.release();
    TempMorphImage.copyTo(ProcessedImage);
    TempMorphImage.release();
    return(ProcessedImage);
}
