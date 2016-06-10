
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

#include "myheader.h"

/*---------------------------------------------------------SMOOTHING OPERATIONS----------------------------------------------------------------*/


/*----------------------------------------------------------HORIZONTAL SMOOTHING----------------------------------------------------------------*/


/**
 * @function horizontal_smoothing
 * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
 * @brief with this function from a foreground position , next whitespace number of pixel is filled in horizontal direction
 * @brief it produce smoothed image of the input image in horizontal direction by filling up with foreground with whitespace amount
 * @return Return horizontally smoothed image of input image(binary in nature)
 */

Mat IITkgp_functions::SmoothingGapfilling::horizontal_smoothing(Mat image, int whitespace)
{
    int i,j,k;
    //int row = image.rows;
    //int col = image.cols;

    Mat hsmoothedimage;

        image.copyTo(hsmoothedimage);

    for(i=0;i<image.rows;i++)
    {
        for(j=0;j<image.cols;j++)
        {
            if(image.data[(i*image.cols)+j]==0)
            {
                for(k=j+1;k<(j+1+whitespace);k++)
                {
                    if(k<image.cols)
                    {
                        hsmoothedimage.data[(i*image.cols)+k]=0;
                    }
                    else
                        break;

                }
            }
        }
    }


    return(hsmoothedimage);

}


/**
 * @function horizontal_gapfilling
 * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
* @brief with this function gap btween two foreground pixel is filled only if the gap between two foreground pixel in horizontal direction has a gap less than or equal to whitespace
 * @brief it produce smoothed image of the input image in horizontal direction by filling up with foreground with whitespace amount
 * @return Return gap-filled image of input image(binary in nature)
 */

Mat IITkgp_functions::SmoothingGapfilling::horizontal_gapfilling(Mat image, int whitespace)
{
    int i,j,k,l;

    Mat hgapfilled;

        image.copyTo(hgapfilled);

    for(i=0;i<image.rows;i++)
    {
      for(j=0;j<image.cols;j++)
      {
        if(image.data[(i*image.cols)+j]==0)
        {
          if( (j+whitespace)<hgapfilled.cols)
          {
        for(k=j+whitespace;k>=j+1;k--)
        {
          if(image.data[(i*image.cols)+k]==0)
          {
            for(l=k;l>=j+1;l--)
              hgapfilled.data[(i*hgapfilled.cols)+l]=0;
            break;
          }
        }
          }
          else
        break;
        }
      }
    }

    return (hgapfilled);
}

/*---------------------------------------------------------VERTICAL SMOOTHING----------------------------------------------------------------*/


/**
 * @function vertical_smoothing
 * @param input an image(binary) in Mat format and integer value whitespace that need to be smoothen or filled up
 * @brief with this function from a foreground position , next whitespace number of pixel is filled
 * @brief it produce smoothed image of the input image in vertical direction by filling up with foreground with whitespace amount
 * @return Return vertically smoothed image of input image(binary in nature)
 */

Mat IITkgp_functions::SmoothingGapfilling::vertical_smoothing(Mat image,int whitespace)
{
    int i,j,k;

    Mat vsmoothedimage;
    image.copyTo(vsmoothedimage);

    for(i=0;i<image.rows;i++)
    {
        for(j=0;j<image.cols;j++)
        {
            if(image.data[(i*image.cols)+j]==0)
            {
                for(k=i+1;k<(i+1+whitespace);k++)
                {
                    if(k<vsmoothedimage.rows)
                    {
                        vsmoothedimage.data[(k*vsmoothedimage.cols)+j]=0;
                    }
                    else
                        break;

                }
            }
        }
    }

    return (vsmoothedimage);
}


/**
 * @function vertical_gapfilling
 * @param input an image(binary) in Mat format and integer value whitespace that need to be gap filled or filled up
 * @brief with this function gap btween two foreground pixel is filled only if the gap between two foreground pixel in vertival direction has a gap less than or equal to whitespace
 * @brief it produce gap filled image of the input image in vertical direction by filling up with foreground with whitespace amount
 * @return Return vertically gap-filled image of input image(binary in nature)
 */


Mat IITkgp_functions::SmoothingGapfilling::vertical_gapfilling(Mat image,int whitespace)
{
    int i,j,k,l;

    Mat vgapfilled;
    image.copyTo(vgapfilled);
    for(i=0;i<image.rows;i++)
    {
      for(j=0;j<image.cols;j++)
      {
        if(image.data[(i*image.cols)+j]==0)
        {
          if( (i+whitespace)<vgapfilled.rows)
          {
        for(k=i+whitespace;k>=i+1;k--)
        {
          if(image.data[(k*image.cols)+j]==0)
          {
            for(l=k;l>=i+1;l--)
              vgapfilled.data[(l*vgapfilled.cols)+j]=0;
            break;
          }
        }
          }
          else
        break;
        }
      }
    }

    return (vgapfilled);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

Mat TempImage,GapImage;
int DirType,SGType;
int hgapsize = 35;
int vgapsize = 5;
int const vmaxgap = 100;
int const hmaxgap = 100;

IITkgp_functions::SmoothingGapfilling u;

void gapped(int, void*)
{


    GapImage = u.vertical_gapfilling(TempImage,vgapsize);
    GapImage = u.horizontal_gapfilling(GapImage,hgapsize);

    imshow("gapfilling Operartion", GapImage);

}


void smooth(int, void*)
{


    GapImage = u.vertical_smoothing(TempImage,vgapsize);
    GapImage = u.horizontal_smoothing(GapImage,hgapsize);

    imshow("smoothing Operartion", GapImage);

}


Mat IITkgp_functions::SmoothingGapfilling::smoothinggapfilling_GUI(Mat image, int sgtype)
{
    image.copyTo(TempImage);

    SGType = sgtype;


    if(SGType == 0) // smoothing
    {
        namedWindow("smoothing Operartion",CV_WINDOW_KEEPRATIO);


        createTrackbar( "HGap:\n ", "smoothing Operartion",
                          &hgapsize, hmaxgap,
                          smooth );
        createTrackbar( "VGap:\n ", "smoothing Operartion",
                          &vgapsize, vmaxgap,
                          smooth );

        smooth(0,0);
        waitKey(0);
    }
    else if(SGType == 1) // GapFilling
    {
        namedWindow("gapfilling Operartion",CV_WINDOW_KEEPRATIO);


        createTrackbar( "HGap:\n ", "gapfilling Operartion",
                          &hgapsize, hmaxgap,
                          gapped );
        createTrackbar( "VGap:\n ", "gapfilling Operartion",
                          &vgapsize, vmaxgap,
                          gapped );

        gapped(0,0);
        waitKey(0);
    }


   TempImage.release();


    return(GapImage);


}
