#pragma once

#include <GL/gl.h>
#include "../../external/Image_Class.h"
#include "../filters/Filter.h"
#include <vector>
#include <queue>
#include <algorithm>

class ImageProcessor {
private:
    GLuint currentTextureID;
    Image currentImage;
    std::vector <Image> undoHistory;
    std::vector <Image> redoHistory;
    int historySize;

    // Selection mask will store whether each pixel is selected (true) or not (false)
    std::vector<std::vector<bool>> selectionMask;
    bool selectionActive = false;
    bool selectionInvertApply = false;

    void pushToHistory(std::vector<Image> &history) {
        // If I exceed history size, remove oldest
        if ((int)history.size() >= historySize) {
            history.erase(history.begin());
        }
        history.push_back(currentImage);
    }

    // Pushes the current image to the undo history, maintaining the history size limit
    void pushUndo() {
        pushToHistory(undoHistory);
    }

    // Pushes the current image to the redo history, maintaining the history size limit
    void pushRedo() {
        pushToHistory(redoHistory);
    }

public:
    ImageProcessor(int hSize = 20) : historySize(hSize) {};

    void setHistorySize(int newSize) {
        historySize = newSize;
        // Trim history when reducing size
        while ((int)undoHistory.size() > historySize) {
            undoHistory.erase(undoHistory.begin());
        }
        while ((int)redoHistory.size() > historySize) {
            redoHistory.erase(redoHistory.begin());
        }
    }

    int getHistorySize() const { return historySize; }

    void loadImage(const std::string &filename) {
        Image newImage(filename);
        currentImage = newImage;
        // When I load a new image, clear history
        undoHistory.clear();
        redoHistory.clear();
    }

    bool saveImage(const std::string &filename) {
        return currentImage.saveImage(filename); 
    }

    void applyFilter(Filter &filter) {
        // When I apply a filter, push current state to undo history and clear redo history
        pushUndo();
        redoHistory.clear();
        filter.apply(currentImage);
    } 

    // Apply a filter only on the selection mask; invert applies to outside
    void applyFilterSelective(Filter &filter, bool invert, bool useHistory = true) {
        if (!hasSelection()) {
            // Fallback to normal apply when no valid selection exists
            applyFilter(filter);
            return;
        }

        if (useHistory) {
            // When I apply a filter, push current state to undo history and clear redo history
            pushUndo();
            redoHistory.clear();
        }

        Image filtered = currentImage;
        filter.apply(filtered);

        // Blend based on selection
        const int imgW = currentImage.width;
        const int imgH = currentImage.height;
        for (int y = 0; y < imgH; ++y) {
            for (int x = 0; x < imgW; ++x) {
                bool selected = selectionMask[y][x];
                if (invert) selected = !selected;
                if (selected) {
                    for (int c = 0; c < 3; ++c) {
                        // If selected, take pixel from filtered image
                        currentImage.setPixel(x, y, c, filtered.getPixel(x, y, c));
                    }
                }
            }
        }
    }

    // Apply selective filter without affecting history (for live previews)
    void applyFilterSelectiveNoHistory(Filter &filter, bool invert) {
        applyFilterSelective(filter, invert, false);
    }

    // Apply a filter without affecting history (for live previews)
    void applyFilterNoHistory(Filter &filter) {
        filter.apply(currentImage);
    }

    // Apply multiple filters as a batch with a single undo entry
    void applyFilterBatch(const std::vector<Filter*>& filters) {
        pushUndo();
        redoHistory.clear();
        for (auto* filter : filters) {
            if (filter) {
                filter->apply(currentImage);
            }
        }
    }

    bool undo() {
        // Return false if no undo available
        if(undoHistory.empty()) return false;
        // If undo is possible, push current state to redo and pop from undo
        pushRedo();
        currentImage = undoHistory.back();
        undoHistory.pop_back();
        return true;
    }

    bool redo() {
        // Return false if no redo available
        if(redoHistory.empty()) return false;
        // If redo is possible, push current state to undo and pop from redo
        pushUndo();
        currentImage = redoHistory.back();
        redoHistory.pop_back();
        return true;
    }

    // Any access from GUI should be read-only for getCurrentImage
    const Image& getCurrentImage() const { return currentImage; }
    void setImage(Image &newImage) { currentImage = newImage; }

    const GLuint& getTextureID() const { return currentTextureID; }

    // Read-only accessors for GUI history rendering
    const std::vector<Image>& getUndoHistory() const { return undoHistory; }
    const std::vector<Image>& getRedoHistory() const { return redoHistory; }

    void clearSelection() {
        selectionMask.clear();
        selectionActive = false;
    }

    bool hasSelection() const {
        if (!selectionActive) return false;
        if (currentImage.width <= 0 || currentImage.height <= 0) return false;
        if ((int)selectionMask.size() != currentImage.height) return false;
        for (const auto &row : selectionMask) {
            if ((int)row.size() != currentImage.width) return false;
        }
        return true;
    }

    const std::vector<std::vector<bool>>& getSelectionMask() const { return selectionMask; }
    bool getSelectionInvertApply() const { return selectionInvertApply; }
    void setSelectionInvertApply(bool invert) { selectionInvertApply = invert; }

    void setRectSelection(int x, int y, int w, int h) {
        if (currentImage.width <= 0 || currentImage.height <= 0) { 
            clearSelection(); 
            return;
        }

        const int imgW = currentImage.width;
        const int imgH = currentImage.height;

        if (x >= imgW || y >= imgH || x + w <= 0 || y + h <= 0) {
            clearSelection();
            return;
        }
        
        int nx = std::clamp(x, 0, imgW - 1);
        int ny = std::clamp(y, 0, imgH - 1);
        int nw = std::clamp(w, 1, imgW - nx);
        int nh = std::clamp(h, 1, imgH - ny);

        selectionMask.assign(imgH, std::vector<bool>(imgW, false));

        for (int yy = ny; yy < ny + nh; ++yy) {
            for (int xx = nx; xx < nx + nw; ++xx) {
                selectionMask[yy][xx] = true;
            }
        }

        selectionActive = true;
    }

    void setMagicWandSelection(int seedX, int seedY, int tolerance) {
        if (currentImage.width <= 0 || currentImage.height <= 0) { 
            clearSelection(); 
            return;
        }

        const int imgW = currentImage.width;
        const int imgH = currentImage.height;

        if (seedX < 0 || seedX >= imgW || seedY < 0 || seedY >= imgH) { 
            clearSelection(); 
            return; 
        }

        selectionMask.assign(imgH, std::vector<bool>(imgW, false));

        int r0 = currentImage.getPixel(seedX, seedY, 0);
        int g0 = currentImage.getPixel(seedX, seedY, 1);
        int b0 = currentImage.getPixel(seedX, seedY, 2);

        std::queue <std::pair<int, int>> q;
        q.push({seedX, seedY});
        selectionMask[seedY][seedX] = true;

        auto withinTol = [&](int r, int g, int b) {
            int dist = std::abs(r - r0) + std::abs(g - g0) + std::abs(b - b0);
            return dist <= tolerance;
        };

        // 4-directional flood fill: right, left, up, down
        const int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        // We'll be using BFS here, the idea is to explore neighbors from the current
        // pixel and add them to the queue if they are within tolerance and not yet selected
        // The visualization is like a seed spreading outwards
        while (!q.empty()) {
            auto [cx, cy] = q.front(); q.pop();
            for (auto &dxy : dir) {
                int nx = cx + dxy[0];
                int ny = cy + dxy[1];

                if (nx < 0 || ny < 0 || nx >= imgW || ny >= imgH || selectionMask[ny][nx]) {
                    continue;
                }

                int r = currentImage.getPixel(nx, ny, 0);
                int g = currentImage.getPixel(nx, ny, 1);
                int b = currentImage.getPixel(nx, ny, 2);
                if (withinTol(r, g, b)) {
                    selectionMask[ny][nx] = true;
                    q.push({nx, ny});
                }
            }
        }

        selectionActive = true;
    }
};