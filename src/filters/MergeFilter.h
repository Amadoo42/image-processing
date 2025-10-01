#pragma once
#include "Filter.h"
#include "ResizeFilter.h"
#include "CropFilter.h"
using namespace std;

class MergeFilter : public Filter {
private:
    Image other_image;
    
public:
    MergeFilter(Image &oth) : Filter("Merge"), other_image(oth) {}

    void apply(Image &image) override {
        // assuming same size
        int x = min(image.width, other_image.width);// rte safe-guard
        int y = min(image.height, other_image.height);
        if(x != image.width || x != other_image.width || y != image.height || y != other_image.height) {
            cout << "images are not the same size \n";
            cout << "1. resize the smaller image to the size of the larger one \n";
            cout << "2. crop the larger image to the size of the smaller image \n";
            while(true) {
                int x; cin >> x;
                int w1, h1, w2, h2;
                w1 = image.width, h1 = image.height;
                w2 = other_image.width, h2 = other_image.height;
                bool z = 0; // z = 0 means first image is smaller
                if(w2 * h2 < w1 * h1) {
                    z = 1;
                }
                if(x == 1) {
                    ResizeFilter resize(max(w1, w2), max(h1, h2));
                    resize.apply(image);
                    resize.apply(other_image);
                    break;
                }else if(x == 2){
                    CropFilter crop(0, 0, min(w1, w2), min(h1, h2));
                    crop.apply(image);
                    crop.apply(other_image);
                    break;
                }else {
                    cout << "invalid input please try again (input 1 or 2) \n";
                }
            }
        }
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                for(int k = 0; k < 3; k++) {
                    image(i, j, k) = (image(i, j, k) + other_image(i, j, k)) / 2;
                }
            }
        }
    }      
};