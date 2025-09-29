#include "../helpers/apply_filter.h"

void Filter::v_flip(Image &image){
    for(int j=0;j<image.height/2;j++){
        for(int i=0;i<image.width;i++){
            swap(image(i,j,0),image(i,image.height-j-1,0));
            swap(image(i,j,1),image(i,image.height-j-1,1));
            swap(image(i,j,2),image(i,image.height-j-1,2));   
        }
    }
}