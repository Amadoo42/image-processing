#include "../helpers/apply_filter.h"

double color(Image &image,int i, int j, int newWidth, int newHeight, int c){
    double srci= i * image.width / newWidth;
    double srcj= j * image.height / newHeight;
            
    int x1 = srci, y1 = srcj, x2 = min(image.width-1,x1+1), y2 = min(image.height-1,y1+1);

    return (x2 - srci) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x1,y1,c)
                    + (srci - x1) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x2,y1,c)
                    + (x2 - srci) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x1,y2,c)
                    + (srci - x1) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x2,y2,c);
}
void Filter::resize(Image &image,int newWidth, int newHeight){
    Image newImage(newWidth,newHeight);
    for(int i=0; i < newWidth; i++){
        for(int j=0; j < newHeight; j++){

            double r = color(image, i, j, newWidth, newHeight, 0);
            double g = color(image, i, j, newWidth, newHeight, 1);
            double b = color(image, i, j, newWidth, newHeight, 2);

            newImage(i,j,0) = r;
            newImage(i,j,1) = g;
            newImage(i,j,2) = b;
        }
    }
    image = newImage;
}
void Filter::resize(Image &image,double ratioX, double ratioY){
    int newWidth = image.width * ratioX;
    int newHeight = image.height * ratioY;
    resize(image, newWidth, newHeight);
}