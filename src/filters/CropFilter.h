#pragma once
#include "Filter.h"
using namespace std;

class CropFilter : public Filter {
private:
    int X, Y, W, H;
public:
    CropFilter(int x, int y, int w, int h) : X(x), Y(y), W(w), H(h) {}

    void apply(Image &image) override {
        Image newImage(W, H);
        for(int i = X; i < X + W; i++) {
            for(int j = Y; j < Y + H; j++) {
                newImage(i - X, j - Y, 0) = image(i, j, 0);
                newImage(i - X, j - Y, 1) = image(i, j, 1);
                newImage(i - X, j - Y, 2) = image(i, j, 2);
            }
        }
        image = newImage;
    }      
};