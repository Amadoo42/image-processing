#pragma once
#include "Filter.h"

class CropFilter : public Filter {
private:
    int X, Y, W, H;
public:
    CropFilter(int x, int y, int w, int h) : X(x), Y(y), W(w), H(h) {}

    void apply(Image &image) override {
        // Validate crop parameters to prevent crashes
        int imgW = image.width;
        int imgH = image.height;
        
        // Clamp crop coordinates to image bounds
        int cropX = std::max(0, std::min(X, imgW));
        int cropY = std::max(0, std::min(Y, imgH));
        int cropW = std::max(1, std::min(W, imgW - cropX));
        int cropH = std::max(1, std::min(H, imgH - cropY));
        
        // Ensure we don't create an empty image
        if (cropW <= 0 || cropH <= 0) {
            cropW = 1;
            cropH = 1;
            cropX = 0;
            cropY = 0;
        }
        
        Image newImage(cropW, cropH);
        for(int i = cropX; i < cropX + cropW; i++) {
            for(int j = cropY; j < cropY + cropH; j++) {
                // Ensure we don't go out of bounds when accessing original image
                int srcX = std::min(i, imgW - 1);
                int srcY = std::min(j, imgH - 1);
                newImage(i - cropX, j - cropY, 0) = image(srcX, srcY, 0);
                newImage(i - cropX, j - cropY, 1) = image(srcX, srcY, 1);
                newImage(i - cropX, j - cropY, 2) = image(srcX, srcY, 2);
            }
        }
        image = newImage;
    }      
};