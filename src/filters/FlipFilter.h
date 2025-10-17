#pragma once
#include "Filter.h"

class FlipFilter : public Filter {
private:
    bool horizontal;
    bool vertical;

public:
    FlipFilter(bool h = false, bool v = false) : horizontal(h), vertical(v) {}
    
    void setHorizontal(bool h) { horizontal = h; }
    void setVertical(bool v) { vertical = v; }
    
    void apply(Image &image) override {
        if (horizontal) {
            // Horizontal flip
            for(int y = 0; y < image.height; ++y) {
                for(int x = 0; x < image.width / 2; ++x) {
                    int mirrorX = image.width - 1 - x;
                    // Swap pixels horizontally
                    for(int c = 0; c < 3; ++c) {
                        unsigned char temp = image(x, y, c);
                        image(x, y, c) = image(mirrorX, y, c);
                        image(mirrorX, y, c) = temp;
                    }
                }
            }
        }
        
        if (vertical) {
            // Vertical flip
            for(int y = 0; y < image.height / 2; ++y) {
                for(int x = 0; x < image.width; ++x) {
                    int mirrorY = image.height - 1 - y;
                    // Swap pixels vertically
                    for(int c = 0; c < 3; ++c) {
                        unsigned char temp = image(x, y, c);
                        image(x, y, c) = image(x, mirrorY, c);
                        image(x, mirrorY, c) = temp;
                    }
                }
            }
        }
    }
};