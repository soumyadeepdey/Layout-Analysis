
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

// Project :  		DRD
// Author : 		Soumyadeep Dey soumyadeepdey@gmail.com
// Creation Date : 	NOV 10  -2014.  Rights Reserved
//~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*~^~*

 

#include "myheader.h"


/*-------------------------------------------------------------------CONNECTED COMPONENT--------------------------------------------------------------------------------*/


int ncc;
int tncc;// total number of connected component
int ncco;//number of connected component in original image

int max_label_forCC;



/**
 * @function labelingdfs
 * @param : input param: input image structure(of (IS(datatype))) input co-ordinate(m,n) to belabeled and image dimention(row,col)
 * @brief : labeling based on DFS algorithm
 * @return : image structure(of (IS(datatype)))
 */



is ** IITkgp_functions::CC_analysis::labelingdfs(is **tempstructuredimage,int *m, int *n, int temp_row, int temp_col)
{

 // printf("hello in DFS and total no of window is %d %d\n",temp_row,temp_col);
  int i,j,l,k;
    i=*m;
    j=*n;
    //printf("iteration \n");
    for(l=i-1;l<=i+1;l++)
    {
      for(k=j-1;k<=j+1;k++)
      {
        IITkgp_functions::PixelValidation vali;
        int temp_val = vali.validate(l,k,temp_row,temp_col);
        if( temp_val == 1)
        {
          if(tempstructuredimage[l][k].label==0 && tempstructuredimage[l][k].value==0)
          {
            tempstructuredimage[l][k].label=tempstructuredimage[i][j].label;
            tempstructuredimage = IITkgp_functions::CC_analysis::labelingdfs(tempstructuredimage,&l,&k,temp_row,temp_col);
          }
        }
      }
    }


    return(tempstructuredimage);
}



// LABELING WITH DFS


/**
 * @function cclabeling
 * @param : input param: input image structure(of (IS(datatype))) and image dimention(row,col)
 * @brief : labeling of ConnectedComponent based on DFS algorithm
 * @return : image structure(of (IS(datatype)))
 */


is** IITkgp_functions::CC_analysis::cclabeling(is **tempstructuredimage,int temp_row, int temp_col)
{
    int label=1;
    int i,j;

    for(i=0;i<temp_row;i++)
    {
        for(j=0;j<temp_col;j++)
        {

            if(tempstructuredimage[i][j].label==0&&tempstructuredimage[i][j].value==0)
            {
                printf("finding connected_component number %d\n",label);
                tempstructuredimage[i][j].label=label;
                tempstructuredimage = IITkgp_functions::CC_analysis::labelingdfs(tempstructuredimage,&i,&j,temp_row,temp_col);
                label=label+1;
            }


        }
    }

    max_label_forCC = label;
    return(tempstructuredimage);

}


/**
 * @function FindConnectedComponent
 * @param : input param: input image(binary) Mat data-type
 *                             src_gray(Gray_scale) Mat data-type
 * @brief : Find connected component from the input binary image
 * @return : array of connected components (nocc(data-type))
 */


nocc* IITkgp_functions::CC_analysis::FindConnectedComponent(Mat image, Mat src_gray)
{
    int row,col;
    int i,j,*mapping,k;
    ncc=0;

    max_label_forCC = 0;


    row = image.rows;
    col = image.cols;

    is **tempstructuredimage;


    tempstructuredimage=(is **)malloc(row * sizeof(is *));
    if(tempstructuredimage == NULL)
    {
      printf("can not allocate space\n");
      exit(0);
    }
    for(i=0;i<row;i++)
    {
        tempstructuredimage[i]=(is *)malloc(col * sizeof(is));
        if(tempstructuredimage[i] == NULL)
        {
          printf("can not allocate space\n");
          exit(0);
        }
    }

    for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            tempstructuredimage[i][j].x=i;
            tempstructuredimage[i][j].y=j;
            tempstructuredimage[i][j].label=0;
            tempstructuredimage[i][j].value=image.data[i*col+j];
        }
    }



// LABELING BY DFS
    printf("before labeling\n");

    tempstructuredimage = IITkgp_functions::CC_analysis::cclabeling(tempstructuredimage,row,col);

    printf("after labeling\n");
    int noccbyla;
    noccbyla = max_label_forCC;

// LABELING IS PERFECT

    mapping=NULL;
    k=0;
    int *tmp,count=1;
    for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            if(tempstructuredimage[i][j].label!=0)
            {

                if(mapping!=NULL)
                {
                    for(k=0;k<count-1;k++)
                    {
                        if(mapping[k]==tempstructuredimage[i][j].label)
                        {
                            tempstructuredimage[i][j].mapped=k;
                            break;
                        }
                    }
                    if(k==count-1)
                    {
                        tmp=(int*)realloc(mapping,count*sizeof(int));
                        if(tmp!=NULL)
                        {
                            mapping=tmp;
                            mapping[count-1]=tempstructuredimage[i][j].label;
                            tempstructuredimage[i][j].mapped=(count-1);
                            count++;
                        }
                        else
                        {
                            printf("\nERROR IN REALLOCATING MAPPING ARREY\n");
                            exit(1);
                        }
                    }// end of k==count
                }// end of if mapping !=null
                else
                {

                    tmp=(int*)realloc(mapping,count*sizeof(int));
                    mapping=tmp;
                    mapping[count-1]=tempstructuredimage[i][j].label;
                    tempstructuredimage[i][j].mapped=(count-1);
                    count++;

                }

            }// end of tempstructuredimage[i][j].label!=0
        }
    }// end of image



// MAPPING IS PERFECR TILL NOW

printf("MAPPING IS PERFECR TILL NOW\n");

    tncc=count-1;

    ncco = tncc;

// CREATING ARREY OF STRUCTURE POINTER  and help them to uniquely mapped

    cc *cctemp,*ccstart=NULL;

    nocc *component;

    component=(nocc *)malloc((count-1)* sizeof(nocc));

    if(component == NULL)
    {
      printf("memory can not be allocated \n");
      exit (0);
    }

    for(i=0;i<(count-1);i++)
    {
        component[i].start=NULL;
        component[i].number_of_component=0;
        component[i].last=NULL;
        for(j=0;j<256;j++)
          component[i].gray_hist[j]=0.0;
    }


    for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            if(tempstructuredimage[i][j].label!=0)
            {

                if(component[tempstructuredimage[i][j].mapped].start==NULL)
                {

                    if(tempstructuredimage[i][j].mapped<0||tempstructuredimage[i][j].mapped>=count-1)
                        {
                            printf("error\n");
                            printf("%d\t%d\t%d\t%d",tempstructuredimage[i][j].mapped,tempstructuredimage[i][j].x,tempstructuredimage[i][j].y,tempstructuredimage[i][j].label);
                            exit(1);
                        }


                    cctemp=(cc *)malloc(sizeof(cc));
                    if(cctemp == NULL)
                    {
                      printf("memory can not be allocated \n");
                      exit (0);
                    }
                    cctemp->im.x=i;
                    cctemp->im.y=j;
                    cctemp->im.label=tempstructuredimage[i][j].label;
                    cctemp->im.mapped=tempstructuredimage[i][j].mapped;
                    cctemp->im.value=src_gray.data[i*col+j];
                    cctemp->nxt=NULL;
                    ccstart=(cc *)malloc(sizeof(cc));
                    ccstart=cctemp;
                    component[tempstructuredimage[i][j].mapped].start=cctemp;
                    component[tempstructuredimage[i][j].mapped].last=cctemp;
                    component[tempstructuredimage[i][j].mapped].number_of_component=1;
                    component[tempstructuredimage[i][j].mapped].xmin=i;
                    component[tempstructuredimage[i][j].mapped].ymin=j;
                    component[tempstructuredimage[i][j].mapped].xmax=i;
                    component[tempstructuredimage[i][j].mapped].ymax=j;
                    component[tempstructuredimage[i][j].mapped].gray_hist[src_gray.data[i*col+j]]=component[tempstructuredimage[i][j].mapped].gray_hist[src_gray.data[i*col+j]]+1;

                }//end of if  i.e. first component of the connected component
                else
                {

                    cctemp=(cc *)malloc(sizeof(cc));
                    if(cctemp == NULL)
                    {
                      printf("memory can not be allocated \n");
                      exit (0);
                    }
                    cctemp->im.x=i;
                    cctemp->im.y=j;
                    cctemp->im.label=tempstructuredimage[i][j].label;
                    cctemp->im.mapped=tempstructuredimage[i][j].mapped;
                    cctemp->im.value=src_gray.data[i*col+j];
                    cctemp->nxt=NULL;
                    if(component[tempstructuredimage[i][j].mapped].last->nxt==NULL)
                        component[tempstructuredimage[i][j].mapped].last->nxt=cctemp;
                    else
                        printf("ERROR\n");
                    component[tempstructuredimage[i][j].mapped].last=cctemp;
                    component[tempstructuredimage[i][j].mapped].number_of_component=(component[tempstructuredimage[i][j].mapped].number_of_component)+1;
                    if(component[tempstructuredimage[i][j].mapped].xmin>i)
                        component[tempstructuredimage[i][j].mapped].xmin=i;
                    if(component[tempstructuredimage[i][j].mapped].ymin>j)
                        component[tempstructuredimage[i][j].mapped].ymin=j;
                    if(component[tempstructuredimage[i][j].mapped].xmax<i)
                        component[tempstructuredimage[i][j].mapped].xmax=i;
                    if(component[tempstructuredimage[i][j].mapped].ymax<j)
                        component[tempstructuredimage[i][j].mapped].ymax=j;

                    component[tempstructuredimage[i][j].mapped].gray_hist[src_gray.data[i*col+j]]=component[tempstructuredimage[i][j].mapped].gray_hist[src_gray.data[i*col+j]]+1;

                }// end of else i.e. not 1st component of connected component
            }// end of if label



        }// end of j



    }// end of i
    printf("CC done\n");
    free(mapping);

    return(component);

}



