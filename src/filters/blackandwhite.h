#include "gray.h"

void blackandwhite(Image &image){
    gray(image);
    for(int i=0;i<image.width;i++){
        for(int j=0;j<image.height;j++){
            if(image(i,j,0)>=128){
                image(i,j,0)=255;
                image(i,j,1)=255;
                image(i,j,2)=255;
            }
            else{
                image(i,j,0)=0;
                image(i,j,1)=0;
                image(i,j,2)=0;
            }
        }
    }
}