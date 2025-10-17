// This will basically manage the current image and provide undo/redo functionality
#pragma once

#include <GL/gl.h>
#include "../../external/Image_Class.h"
#include "../filters/Filter.h"
#include <vector>
#include <string>
#include <queue>
#include <cstdint>
#include <algorithm>
#include <cstdlib>


class ImageProcessor {
private:
    GLuint currentTextureID;
    Image currentImage;
    std::vector <Image> undoHistory;
    std::vector <Image> redoHistory;
    int historySize;

    // Selection state (mask same size as current image; 1 = selected, 0 = not selected)
    std::vector<uint8_t> selectionMask;
    bool selectionActive = false;
    bool selectionInvertApply = false; // when true, apply to outside of selection

    void pushUndo() {
        if((int)undoHistory.size() >= historySize) undoHistory.erase(undoHistory.begin());
        undoHistory.push_back(currentImage);
    }

    void pushRedo() {
        if((int)redoHistory.size() >= historySize) redoHistory.erase(redoHistory.begin());
        redoHistory.push_back(currentImage);
    }

public:
    ImageProcessor(int hSize = 20) : historySize(hSize) {};

    void loadImage(const std::string &filename) {
        Image newImage(filename);
        currentImage = newImage;
        undoHistory.clear();
        redoHistory.clear();
    }

    bool saveImage(const std::string &filename) {
        return currentImage.saveImage(filename); 
    }

    void applyFilter(Filter &filter) {
        pushUndo();
        redoHistory.clear();
        filter.apply(currentImage);
        //textureNeedsUpdate = true;
    } 

    // Apply a filter only on the current selection mask. If invert=true,
    // the filter is applied outside the selection instead.
    void applyFilterSelective(Filter &filter, bool invert) {
        if (currentImage.width <= 0 || currentImage.height <= 0) return;
        if (!selectionActive || selectionMask.size() != (size_t)(currentImage.width * currentImage.height)) {
            // Fallback to normal apply when no valid selection exists
            applyFilter(filter);
            return;
        }

        pushUndo();
        redoHistory.clear();

        Image original = currentImage;       // snapshot
        Image filtered = currentImage;       // working copy
        filter.apply(filtered);              // run filter on whole image

        // Blend based on selection
        const int w = currentImage.width;
        const int h = currentImage.height;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                size_t idx = (size_t)y * (size_t)w + (size_t)x;
                bool sel = selectionMask[idx] != 0;
                if (invert) sel = !sel;
                if (sel) {
                    // copy filtered pixel
                    for (int c = 0; c < 3; ++c) {
                        currentImage.setPixel(x, y, c, filtered.getPixel(x, y, c));
                    }
                } else {
                    // copy original pixel (already is, but be explicit)
                    for (int c = 0; c < 3; ++c) {
                        currentImage.setPixel(x, y, c, original.getPixel(x, y, c));
                    }
                }
            }
        }
    }

    // No-history version of selective application for live previews
    void applyFilterSelectiveNoHistory(Filter &filter, bool invert) {
        if (currentImage.width <= 0 || currentImage.height <= 0) return;
        if (!selectionActive || selectionMask.size() != (size_t)(currentImage.width * currentImage.height)) {
            // Fallback to normal no-history apply
            filter.apply(currentImage);
            return;
        }
        Image original = currentImage;
        Image filtered = currentImage;
        filter.apply(filtered);
        const int w = currentImage.width;
        const int h = currentImage.height;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                size_t idx = (size_t)y * (size_t)w + (size_t)x;
                bool sel = selectionMask[idx] != 0;
                if (invert) sel = !sel;
                if (sel) {
                    for (int c = 0; c < 3; ++c) {
                        currentImage.setPixel(x, y, c, filtered.getPixel(x, y, c));
                    }
                } else {
                    for (int c = 0; c < 3; ++c) {
                        currentImage.setPixel(x, y, c, original.getPixel(x, y, c));
                    }
                }
            }
        }
    }

    // Apply a filter without affecting undo/redo history.
    // Intended for live previews; caller should manage resetting or committing.
    void applyFilterNoHistory(Filter &filter) {
        filter.apply(currentImage);
    }

    bool undo() {
        if(undoHistory.empty()) return false;
        pushRedo();
        currentImage = undoHistory.back();
        undoHistory.pop_back();
        return true;
    }

    bool redo() {
        if(redoHistory.empty()) return false;
        pushUndo();
        currentImage = redoHistory.back();
        redoHistory.pop_back();
        return true;
    }

    const Image& getCurrentImage() const { return currentImage; }
    void setImage(Image &newImage) { currentImage = newImage; }

    const GLuint& getTextureID() const { return currentTextureID; }

    // Expose read-only accessors for GUI visual history rendering
    const std::vector<Image>& getUndoHistory() const { return undoHistory; }
    const std::vector<Image>& getRedoHistory() const { return redoHistory; }

    // --- Selection API ------------------------------------------------------
    void clearSelection() {
        selectionMask.clear();
        selectionActive = false;
    }

    bool hasSelection() const {
        return selectionActive && selectionMask.size() == (size_t)(currentImage.width * currentImage.height);
    }

    const std::vector<uint8_t>& getSelectionMask() const { return selectionMask; }
    bool getSelectionInvertApply() const { return selectionInvertApply; }
    void setSelectionInvertApply(bool invert) { selectionInvertApply = invert; }

    void setRectSelection(int x, int y, int w, int h) {
        if (currentImage.width <= 0 || currentImage.height <= 0) { clearSelection(); return; }
        int imgW = currentImage.width;
        int imgH = currentImage.height;
        int rx = std::max(0, std::min(x, imgW - 1));
        int ry = std::max(0, std::min(y, imgH - 1));
        int rw = std::max(1, w);
        int rh = std::max(1, h);
        if (rx + rw > imgW) rw = imgW - rx;
        if (ry + rh > imgH) rh = imgH - ry;
        selectionMask.assign((size_t)imgW * (size_t)imgH, 0);
        for (int yy = ry; yy < ry + rh; ++yy) {
            size_t row = (size_t)yy * (size_t)imgW;
            for (int xx = rx; xx < rx + rw; ++xx) {
                selectionMask[row + (size_t)xx] = 1;
            }
        }
        selectionActive = true;
    }

    void setMagicWandSelection(int seedX, int seedY, int tolerance) {
        if (currentImage.width <= 0 || currentImage.height <= 0) { clearSelection(); return; }
        int imgW = currentImage.width;
        int imgH = currentImage.height;
        if (seedX < 0 || seedX >= imgW || seedY < 0 || seedY >= imgH) { clearSelection(); return; }
        selectionMask.assign((size_t)imgW * (size_t)imgH, 0);

        const unsigned char* data = currentImage.imageData;
        const int channels = currentImage.channels; // expected 3
        auto idxOf = [&](int X, int Y){ return ((size_t)Y * (size_t)imgW + (size_t)X); };
        auto pixOf = [&](int X, int Y){ return ((size_t)Y * (size_t)imgW + (size_t)X) * (size_t)channels; };

        size_t seedPi = pixOf(seedX, seedY);
        int r0 = data[seedPi + 0];
        int g0 = data[seedPi + 1];
        int b0 = data[seedPi + 2];

        std::queue<std::pair<int,int>> q;
        q.push({seedX, seedY});
        selectionMask[idxOf(seedX, seedY)] = 1;

        auto withinTol = [&](int r, int g, int b){
            int d = std::abs(r - r0) + std::abs(g - g0) + std::abs(b - b0);
            return d <= tolerance; // tolerance in [0..765] typical 30..60 works well
        };

        const int dir4[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        while(!q.empty()) {
            auto [cx, cy] = q.front(); q.pop();
            for (auto &dxy : dir4) {
                int nx = cx + dxy[0];
                int ny = cy + dxy[1];
                if (nx < 0 || ny < 0 || nx >= imgW || ny >= imgH) continue;
                size_t i = idxOf(nx, ny);
                if (selectionMask[i]) continue;
                size_t p = pixOf(nx, ny);
                int r = data[p + 0];
                int g = data[p + 1];
                int b = data[p + 2];
                if (withinTol(r, g, b)) {
                    selectionMask[i] = 1;
                    q.push({nx, ny});
                }
            }
        }
        selectionActive = true;
    }
};