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
    bool useOverlayPlacement = false;

public:
    // Legacy: merge full images by averaging overlapping pixels
    MergeFilter(Image &oth) : other_image(oth), useOverlayPlacement(false) {}

    // Overlay: place and blend other_image over base at (x,y) with given size and alpha
    MergeFilter(Image &oth, int x, int y, int w, int h, float a)
        : other_image(oth), posX(x), posY(y), targetWidth(w), targetHeight(h), alpha(a), useOverlayPlacement(true) {}

    void apply(Image &image) override {
        if (useOverlayPlacement) {
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
                        image(x, y, c) = (unsigned char)(base_v * (1.0f - alpha) + over_v * alpha);
                    }
                }
            }
            return;
        }

        // Fallback: average overlapping area automatically without interactive input
        int w = std::min(image.width, other_image.width);
        int h = std::min(image.height, other_image.height);

        Image base_resized = image;
        Image other_resized = other_image;
        if (image.width != w || image.height != h) {
            ResizeFilter r(w, h);
            r.apply(base_resized);
        }
        if (other_image.width != w || other_image.height != h) {
            ResizeFilter r(w, h);
            r.apply(other_resized);
        }

        Image out(w, h);
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                for (int k = 0; k < 3; ++k) {
                    out(i, j, k) = (base_resized(i, j, k) + other_resized(i, j, k)) / 2;
                }
            }
        }
        image = out;
    }
};