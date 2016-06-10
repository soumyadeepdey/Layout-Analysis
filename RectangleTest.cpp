
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

#include "myheader.h"

/*------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------*/


/**
 * @Function: PointRectangleTest
 * @brief : Take 1 rectangle and a Point as input
 * @brief : Test whether the Given Point is inside the Given Rectangle or Inside
 * @return : 	0: Point is Outside of Rectangle
 * 		1: Point is inside the given Rectangle
 * */

int IITkgp_functions::RectangleTest::PointRectangleTest(Rect GivenRect, Point GivenPoint)
{
  Point tl,br;
  tl = GivenRect.tl();
  br = GivenRect.br();
  int flag;
  /*
  if((GivenPoint.x>=tl.x && GivenPoint.x<=br.x) && (GivenPoint.y<=tl.y && GivenPoint.y>=br.y))
  {
    flag = 1;
    printf("point inside\n");
    return(flag);
  }
  */
  if((GivenPoint.x>=tl.x && GivenPoint.x<=br.x) && (GivenPoint.y>=tl.y && GivenPoint.y<=br.y))
  {
    flag = 1;
    //printf("point inside\n");
    return(flag);
  }
  else
  {
    flag = 0;
    return(flag);
  }
}

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

int IITkgp_functions::RectangleTest::FindOverlappingRectangles(Rect first, Rect second)
{

 Point tl1,tl2,br1,br2;

 int flag;

 tl1 = first.tl();
 tl2 = second.tl();
 br1 = first.br();
 br2 = second.br();

 if(PointRectangleTest(first,tl2) == 0 && PointRectangleTest(first,br2) == 0)
 {
   flag = 0;
   //return (flag);
 }

 if(PointRectangleTest(first,tl2) == 1 || PointRectangleTest(first,br2) ==1 || PointRectangleTest(second,tl1) == 1 || PointRectangleTest(second,br1) == 1)
 {
   flag = 3;
   //return (flag);
 }
 if(PointRectangleTest(first,tl2) == 1 && PointRectangleTest(first,br2) == 1)
 {
   flag = 2;
   //return (flag);
 }
 if(PointRectangleTest(second,tl1) == 1 && PointRectangleTest(second,br1) == 1)
 {
   flag = 1;
   //return (flag);
 }
 return (flag);

}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/
