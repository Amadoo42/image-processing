#pragma once
#include "Filter.h"
using namespace std;

class ResizeFilter : public Filter {
private:
    int newWidth, newHeight;
    double ratioX, ratioY;
    bool useRatio = false;

    double color(Image &image,int i, int j, int newWidth, int newHeight, int c){
        double srci= i * image.width / newWidth;
        double srcj= j * image.height / newHeight;
                
        int x1 = srci, y1 = srcj, x2 = min(image.width-1,x1+1), y2 = min(image.height-1,y1+1);

        return (x2 - srci) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x1,y1,c)
                        + (srci - x1) * (y2 - srcj) / ((x2 - x1) * (y2 -y1)) * image(x2,y1,c)
                        + (x2 - srci) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x1,y2,c)
                        + (srci - x1) * (srcj - y1) / ((x2 - x1) * (y2 -y1)) * image(x2,y2,c);
    }

public:
    ResizeFilter(int nW, int nH) : newWidth(nW), newHeight(nH), useRatio(false) {}
    ResizeFilter(double rX, double rY) : ratioX(rX), ratioY(rY), useRatio(true) {}

    void apply(Image &image) override {
        if(useRatio) {
            newWidth = image.width * ratioX;
            newHeight = image.height * ratioY;
            cout << newHeight << endl;
        }
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
};