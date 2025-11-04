#pragma once
#include "Filter.h"
#include "ResizeFilter.h"
#include "CropFilter.h"
#include <algorithm>

class MergeFilter : public Filter {
private:
    Image other_image;
    int posX = 0;
    int posY = 0;
    int targetWidth = 0;
    int targetHeight = 0;
    float alpha = 0.5f;

public:
    // Remove this later:
    MergeFilter(Image &oth) : other_image(oth) {}
    // Overlay: place and blend other_image over base at (x,y) with given size and alpha
    MergeFilter(Image &oth, int x, int y, int w, int h, float a)
        : other_image(oth), posX(x), posY(y), targetWidth(w), targetHeight(h), alpha(a) {}

    void apply(Image &image) override {
        if (targetWidth <= 0 || targetHeight <= 0) return;
        Image overlay_resized = other_image;
        ResizeFilter resize_overlay(targetWidth, targetHeight);
        resize_overlay.apply(overlay_resized);

        int x0 = std::max(0, posX);
        int y0 = std::max(0, posY);
        int x1 = std::min(image.width, posX + overlay_resized.width);
        int y1 = std::min(image.height, posY + overlay_resized.height);
        if (x1 <= x0 || y1 <= y0) return;

        for (int y = y0; y < y1; ++y) {
            for (int x = x0; x < x1; ++x) {
                int ox = x - posX;
                int oy = y - posY;
                for (int c = 0; c < 3; ++c) {
                    float base_v = image(x, y, c);
                    float over_v = overlay_resized(ox, oy, c);
                    // We use linear interpolation for blending
                    image(x, y, c) = (base_v * (1.0f - alpha) + over_v * alpha);
                }
            }
        }
    }
};