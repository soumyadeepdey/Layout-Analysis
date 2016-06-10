
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*


#include "myheader.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------*/






/**
 * @function validate
 * @param : input param: co-ordinate position(i,j) and maximum-limit(t) row, col
 * @brief : co-ordinate position(i,j) to be check whether it is within given row and col
 * @return : 1 if it belong to particular region
 *           0 if not belong within that particular row and col
 */


int IITkgp_functions::PixelValidation::validate(int i, int j, int row, int col)
{

  if(i<0 || i>=row || j<0 || j>=col)
    return 0;
  else
    return 1;
}

