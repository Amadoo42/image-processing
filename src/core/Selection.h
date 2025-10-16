#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include "../../external/Image_Class.h"

struct Point { int x; int y; };

class Selection {
public:
    enum ToolMode { NONE, RECTANGLE, LASSO, MAGIC_WAND };

    ToolMode currentTool = NONE;
    Image mask; // single-channel mask 0..255, same W/H as composite

    Selection() = default;

    void ensureMaskSize(int w, int h) {
        if (mask.width != w || mask.height != h || mask.channels != 3) {
            // We use channels=1 logically; but Image_Class forces 3. We'll store gray
            mask = Image(w, h);
            // initialize to 0
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w; ++x)
                    for (int c = 0; c < 3; ++c)
                        mask(x, y, c) = 0;
        }
    }

    void clear() {
        if (mask.width <= 0 || mask.height <= 0) return;
        for (int y = 0; y < mask.height; ++y)
            for (int x = 0; x < mask.width; ++x)
                for (int c = 0; c < 3; ++c)
                    mask(x, y, c) = 0;
    }

    void setMaskFromRectangle(int x, int y, int w, int h) {
        if (mask.width <= 0 || mask.height <= 0) return;
        int x0 = std::max(0, x);
        int y0 = std::max(0, y);
        int x1 = std::min(mask.width, x + w);
        int y1 = std::min(mask.height, y + h);
        for (int yy = y0; yy < y1; ++yy) {
            for (int xx = x0; xx < x1; ++xx) {
                unsigned char v = 255;
                for (int c = 0; c < 3; ++c) mask(xx, yy, c) = v;
            }
        }
    }

    void setMaskFromPolygon(const std::vector<Point>& poly) {
        if (mask.width <= 0 || mask.height <= 0 || poly.size() < 3) return;
        // Simple scanline fill algorithm for polygon
        for (int y = 0; y < mask.height; ++y) {
            std::vector<int> nodes;
            int j = (int)poly.size() - 1;
            for (size_t i = 0; i < poly.size(); ++i) {
                int yi = poly[i].y, yj = poly[j].y;
                int xi = poly[i].x, xj = poly[j].x;
                bool cond = (yi < y && yj >= y) || (yj < y && yi >= y);
                if (cond && (yi != yj)) {
                    int xint = xi + (y - yi) * (xj - xi) / (yj - yi);
                    nodes.push_back(xint);
                }
                j = (int)i;
            }
            std::sort(nodes.begin(), nodes.end());
            for (size_t k = 0; k + 1 < nodes.size(); k += 2) {
                int xStart = std::max(0, nodes[k]);
                int xEnd = std::min(mask.width, nodes[k + 1]);
                for (int x = xStart; x < xEnd; ++x) {
                    for (int c = 0; c < 3; ++c) mask(x, y, c) = 255;
                }
            }
        }
    }

    void setMaskFromMagicWand(int startX, int startY, float tolerance) {
        if (mask.width <= 0 || mask.height <= 0) return;
        // Flood fill on current mask image using tolerance on external reference is not provided here.
        // Placeholder: mark a small disk region around the start point.
        int radius = std::max(1, (int)std::round(tolerance));
        for (int y = std::max(0, startY - radius); y < std::min(mask.height, startY + radius + 1); ++y) {
            for (int x = std::max(0, startX - radius); x < std::min(mask.width, startX + radius + 1); ++x) {
                int dx = x - startX, dy = y - startY;
                if (dx*dx + dy*dy <= radius*radius) {
                    for (int c = 0; c < 3; ++c) mask(x, y, c) = 255;
                }
            }
        }
    }
};
