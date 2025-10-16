// This will manage the image, provide undo/redo, selection, and basic layers
#pragma once

#include <GL/gl.h>
#include "../../external/Image_Class.h"
#include "../filters/Filter.h"
#include <vector>
#include <string>
#include <queue>
#include <optional>
#include <utility>
#include <algorithm>
#include <cmath>


class ImageProcessor {
public:
    struct Layer {
        Image content;         // RGB image, same dimensions as canvas
        std::string name;      // UI name
        bool visible = true;   // visibility toggle
        float opacity = 1.0f;  // uniform opacity [0,1]
    };

private:
    // Rendering state
    GLuint currentTextureID = 0; // not actively used by core; kept for compatibility
    Image currentImage;          // Composite image used for display/saving

    // Undo/redo store composite snapshots (for history thumbnails)
    std::vector<Image> undoHistory;
    std::vector<Image> redoHistory;
    // Layer graph snapshots to make undo/redo correct
    std::vector<std::vector<Layer>> layersUndoHistory;
    std::vector<std::vector<Layer>> layersRedoHistory;
    int historySize;

    // Layers and active index
    std::vector<Layer> layers;
    int activeLayerIndex = 0;

    // Selection mask (0=unselected, 255=selected)
    std::vector<unsigned char> selectionMask; // width*height when active
    bool selectionActive = false;
    bool selectionInvert = false; // apply effects outside selection when true

    // No separate preview buffer; previews render into currentImage temporarily

private:
    inline int idxOf(int x, int y, int width) const { return y * width + x; }

    void ensureMaskSize() {
        if (!hasImage()) return;
        size_t need = (size_t)currentImage.width * (size_t)currentImage.height;
        if (selectionMask.size() != need) selectionMask.assign(need, 0);
    }

    void pushUndo() {
        if ((int)undoHistory.size() >= historySize) undoHistory.erase(undoHistory.begin());
        if ((int)layersUndoHistory.size() >= historySize) layersUndoHistory.erase(layersUndoHistory.begin());
        // Store composite snapshot for thumbnails and matching layers snapshot
        undoHistory.push_back(currentImage);
        layersUndoHistory.push_back(layers); // deep copy via Image::operator=
    }

    void pushRedo() {
        if ((int)redoHistory.size() >= historySize) redoHistory.erase(redoHistory.begin());
        if ((int)layersRedoHistory.size() >= historySize) layersRedoHistory.erase(layersRedoHistory.begin());
        redoHistory.push_back(currentImage);
        layersRedoHistory.push_back(layers);
    }

    static unsigned char blendChannel(unsigned char a, unsigned char b, float t) {
        // linear interpolate between a (dst) and b (src) by t
        float res = (1.0f - t) * (float)a + t * (float)b;
        int ir = (int)std::round(res);
        if (ir < 0) ir = 0; else if (ir > 255) ir = 255;
        return (unsigned char)ir;
    }

    void compositeLayers() {
        if (layers.empty()) return;

        // Start with bottom-most visible layer or black if none visible
        int w = layers[0].content.width;
        int h = layers[0].content.height;
        if (currentImage.width != w || currentImage.height != h) {
            currentImage = Image(w, h);
            // initialize black
            std::fill(currentImage.imageData, currentImage.imageData + w * h * 3, 0);
        }

        // Initialize with black
        std::fill(currentImage.imageData, currentImage.imageData + (size_t)w * (size_t)h * 3, 0);

        // Accumulate from bottom to top using uniform opacity
        for (size_t li = 0; li < layers.size(); ++li) {
            const Layer &L = layers[li];
            if (!L.visible || L.opacity <= 0.0f) continue;
            const unsigned char *src = L.content.imageData;
            unsigned char *dst = currentImage.imageData;
            float t = std::clamp(L.opacity, 0.0f, 1.0f);
            size_t N = (size_t)w * (size_t)h;
            for (size_t i = 0; i < N; ++i) {
                size_t o = i * 3;
                dst[o + 0] = blendChannel(dst[o + 0], src[o + 0], t);
                dst[o + 1] = blendChannel(dst[o + 1], src[o + 1], t);
                dst[o + 2] = blendChannel(dst[o + 2], src[o + 2], t);
            }
        }
    }

    bool hasImage() const { return currentImage.width > 0 && currentImage.height > 0; }

    // Apply selection gating to "dst" using original snapshot "orig"
    // Keep filtered pixels only where mask says to apply (or outside when inverted)
    void applySelectionGate(Image &dst, const Image &orig) {
        if (!selectionActive || selectionMask.empty()) return; // nothing to gate
        const int w = dst.width;
        const int h = dst.height;
        const size_t N = (size_t)w * (size_t)h;
        for (size_t i = 0; i < N; ++i) {
            bool selected = selectionMask[i] > 0;
            bool keepFiltered = selectionInvert ? !selected : selected; // invert means apply outside selection
            if (!keepFiltered) {
                size_t o = i * 3;
                dst.imageData[o + 0] = orig.imageData[o + 0];
                dst.imageData[o + 1] = orig.imageData[o + 1];
                dst.imageData[o + 2] = orig.imageData[o + 2];
            }
        }
    }

public:
    ImageProcessor(int hSize = 10) : historySize(hSize) {}

    // -------- Image I/O --------
    void loadImage(const std::string &filename) {
        Image newImage(filename);
        // Initialize as a single background layer
        layers.clear();
        Layer bg; bg.content = newImage; bg.name = "Background"; bg.visible = true; bg.opacity = 1.0f;
        layers.push_back(bg);
        activeLayerIndex = 0;
        compositeLayers();

        undoHistory.clear();
        redoHistory.clear();
        selectionMask.clear(); selectionActive = false; selectionInvert = false;
        layersUndoHistory.clear(); layersRedoHistory.clear();
    }

    bool saveImage(const std::string &filename) {
        // Save the real composite, not a preview override
        return currentImage.saveImage(filename);
    }

    // Current composite for display
    const Image& getCurrentImage() const { return currentImage; }

    // Set current composite (used by previews). Does not mutate layers.
    void setImage(Image &newImage) { currentImage = newImage; }

    const GLuint& getTextureID() const { return currentTextureID; }

    // -------- Layers API (basic) --------
    int getLayerCount() const { return (int)layers.size(); }
    int getActiveLayer() const { return activeLayerIndex; }
    void setActiveLayer(int idx) { if (idx >= 0 && idx < (int)layers.size()) activeLayerIndex = idx; }
    const Layer& getLayer(int idx) const { return layers[idx]; }
    Layer& getLayer(int idx) { return layers[idx]; }

    // Create a new blank layer (black) on top with 0 opacity by default
    int newLayer(const std::string &name = "Layer") {
        if (!hasImage()) return -1;
        pushUndo();
        redoHistory.clear(); layersRedoHistory.clear();
        Layer L;
        L.content = Image(currentImage.width, currentImage.height);
        std::fill(L.content.imageData, L.content.imageData + (size_t)L.content.width * (size_t)L.content.height * 3, 0);
        L.name = name;
        L.visible = true;
        L.opacity = 0.0f; // start invisible to avoid covering background
        layers.push_back(L);
        activeLayerIndex = (int)layers.size() - 1;
        compositeLayers();
        return activeLayerIndex;
    }

    // Duplicate an existing layer
    int duplicateLayer(int idx) {
        if (idx < 0 || idx >= (int)layers.size()) return -1;
        pushUndo();
        redoHistory.clear(); layersRedoHistory.clear();
        Layer copy = layers[idx];
        copy.name += " Copy";
        layers.insert(layers.begin() + idx + 1, copy);
        activeLayerIndex = idx + 1;
        compositeLayers();
        return activeLayerIndex;
    }

    void deleteLayer(int idx) {
        if (layers.size() <= 1) return; // always keep at least one
        if (idx < 0 || idx >= (int)layers.size()) return;
        pushUndo();
        redoHistory.clear(); layersRedoHistory.clear();
        layers.erase(layers.begin() + idx);
        if (activeLayerIndex >= (int)layers.size()) activeLayerIndex = (int)layers.size() - 1;
        compositeLayers();
    }

    void moveLayer(int from, int to) {
        if (from < 0 || from >= (int)layers.size() || to < 0 || to >= (int)layers.size() || from == to) return;
        pushUndo();
        redoHistory.clear(); layersRedoHistory.clear();
        Layer tmp = layers[from];
        layers.erase(layers.begin() + from);
        layers.insert(layers.begin() + to, tmp);
        activeLayerIndex = to;
        compositeLayers();
    }

    void setLayerVisibility(int idx, bool vis) {
        if (idx >= 0 && idx < (int)layers.size()) {
            pushUndo(); redoHistory.clear(); layersRedoHistory.clear();
            layers[idx].visible = vis; compositeLayers();
        }
    }
    void setLayerOpacity(int idx, float op) {
        if (idx >= 0 && idx < (int)layers.size()) {
            pushUndo(); redoHistory.clear(); layersRedoHistory.clear();
            layers[idx].opacity = std::clamp(op, 0.0f, 1.0f); compositeLayers();
        }
    }

    // -------- Selection API --------
    bool hasActiveSelection() const { return selectionActive; }
    void clearSelection() { selectionMask.clear(); selectionActive = false; selectionInvert = false; }
    void setSelectionInvert(bool inv) { selectionInvert = inv; }
    bool getSelectionInvert() const { return selectionInvert; }

    // Rectangle selection (x,y,w,h in image pixels)
    void setRectSelection(int x, int y, int w, int h) {
        if (!hasImage()) return;
        ensureMaskSize();
        std::fill(selectionMask.begin(), selectionMask.end(), 0);
        int W = currentImage.width, H = currentImage.height;
        int x0 = std::max(0, std::min(x, W - 1));
        int y0 = std::max(0, std::min(y, H - 1));
        int x1 = std::max(0, std::min(x + w, W));
        int y1 = std::max(0, std::min(y + h, H));
        for (int j = y0; j < y1; ++j) {
            for (int i = x0; i < x1; ++i) selectionMask[idxOf(i, j, W)] = 255;
        }
        selectionActive = (w > 0 && h > 0);
    }

    // Polygon selection using even-odd fill rule; pts are image pixel coordinates
    void setPolygonSelection(const std::vector<std::pair<int,int>> &pts) {
        if (!hasImage() || pts.size() < 3) { clearSelection(); return; }
        ensureMaskSize();
        std::fill(selectionMask.begin(), selectionMask.end(), 0);
        int W = currentImage.width, H = currentImage.height;
        int minY = H, maxY = 0;
        for (auto &p : pts) { minY = std::min(minY, std::max(0, std::min(H - 1, p.second))); maxY = std::max(maxY, std::max(0, std::min(H - 1, p.second))); }
        for (int y = minY; y <= maxY; ++y) {
            std::vector<int> nodes;
            size_t n = pts.size();
            for (size_t i = 0, j = n - 1; i < n; j = i++) {
                int yi = pts[i].second, yj = pts[j].second;
                int xi = pts[i].first,  xj = pts[j].first;
                bool cond = ((yi < y && yj >= y) || (yj < y && yi >= y));
                if (cond) {
                    // compute x intersection
                    float xint = xi + (float)(y - yi) * (float)(xj - xi) / (float)((yj - yi) == 0 ? 1 : (yj - yi));
                    nodes.push_back((int)std::floor(xint));
                }
            }
            std::sort(nodes.begin(), nodes.end());
            for (size_t k = 0; k + 1 < nodes.size(); k += 2) {
                int xStart = std::max(0, std::min(W - 1, nodes[k]));
                int xEnd   = std::max(0, std::min(W - 1, nodes[k + 1]));
                if (xEnd < xStart) std::swap(xStart, xEnd);
                for (int x = xStart; x <= xEnd; ++x) selectionMask[idxOf(x, y, W)] = 255;
            }
        }
        selectionActive = true;
    }

    // Magic wand selection using flood fill with tolerance [0..255]
    void setMagicWandSelection(int seedX, int seedY, int tolerance) {
        if (!hasImage()) return;
        ensureMaskSize();
        std::fill(selectionMask.begin(), selectionMask.end(), 0);
        int W = currentImage.width, H = currentImage.height;
        seedX = std::clamp(seedX, 0, W - 1); seedY = std::clamp(seedY, 0, H - 1);
        const unsigned char *data = currentImage.imageData;
        auto at = [&](int x, int y, int c){ return data[(y * W + x) * 3 + c]; };
        int sr = at(seedX, seedY, 0), sg = at(seedX, seedY, 1), sb = at(seedX, seedY, 2);
        auto diff = [&](int r, int g, int b){ return std::abs(r - sr) + std::abs(g - sg) + std::abs(b - sb); };
        int thresh = std::clamp(tolerance, 0, 255) * 3; // scale across 3 channels

        std::queue<std::pair<int,int>> q;
        std::vector<unsigned char> visited((size_t)W * (size_t)H, 0);
        q.push({seedX, seedY}); visited[idxOf(seedX, seedY, W)] = 1;
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            int r = at(x, y, 0), g = at(x, y, 1), b = at(x, y, 2);
            if (diff(r, g, b) <= thresh) {
                selectionMask[idxOf(x, y, W)] = 255;
                const int dx[4] = {1,-1,0,0};
                const int dy[4] = {0,0,1,-1};
                for (int k = 0; k < 4; ++k) {
                    int nx = x + dx[k], ny = y + dy[k];
                    if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
                        size_t id = idxOf(nx, ny, W);
                        if (!visited[id]) { visited[id] = 1; q.push({nx, ny}); }
                    }
                }
            }
        }
        selectionActive = true;
    }

    // Return bounding rect of current selection if any
    bool getSelectionBounds(int &x, int &y, int &w, int &h) const {
        if (!selectionActive || selectionMask.empty() || !hasImage()) return false;
        int W = currentImage.width, H = currentImage.height;
        int minX = W, minY = H, maxX = -1, maxY = -1;
        for (int j = 0; j < H; ++j) {
            for (int i = 0; i < W; ++i) {
                if (selectionMask[(size_t)j * (size_t)W + (size_t)i]) {
                    if (i < minX) minX = i; if (i > maxX) maxX = i;
                    if (j < minY) minY = j; if (j > maxY) maxY = j;
                }
            }
        }
        if (maxX < minX || maxY < minY) return false;
        x = minX; y = minY; w = maxX - minX + 1; h = maxY - minY + 1;
        return true;
    }

    // -------- Filters application --------
    void applyFilter(Filter &filter) {
        if (!hasImage()) return;
        // Commit point
        pushUndo();
        redoHistory.clear(); layersRedoHistory.clear();

        // Apply to active layer content first to inspect result size
        int idx = std::clamp(getActiveLayer(), 0, (int)layers.size() - 1);
        Image beforeLayer = layers[idx].content; // copy
        Image testLayer = beforeLayer;           // working copy
        filter.apply(testLayer);

        const bool sizeChanged = (testLayer.width != beforeLayer.width) || (testLayer.height != beforeLayer.height);
        if (sizeChanged) {
            // Structural transform: apply to ALL layers without selection gating
            for (size_t i = 0; i < layers.size(); ++i) {
                Image tmp = layers[i].content; // copy
                filter.apply(tmp);
                layers[i].content = tmp;
            }
            compositeLayers();
            return;
        }

        // Effects-like edit: apply to active layer and gate by selection
        Image afterLayer = testLayer; // already filtered
        applySelectionGate(afterLayer, beforeLayer);
        layers[idx].content = afterLayer; // commit
        compositeLayers();
    }

    // Apply a filter without affecting undo/redo history. Intended for live previews.
    void applyFilterNoHistory(Filter &filter) {
        if (!hasImage()) return;
        // Preview by synthesizing a temporary layer set
        int idx = std::clamp(getActiveLayer(), 0, (int)layers.size() - 1);
        Image beforeLayer = layers[idx].content; // copy
        Image previewLayer = beforeLayer;
        filter.apply(previewLayer);

        const bool sizeChanged = (previewLayer.width != beforeLayer.width) || (previewLayer.height != beforeLayer.height);

        // Save originals to restore after composing preview
        std::vector<Layer> saved = layers;

        if (sizeChanged) {
            // Structural: apply preview to all layers
            for (size_t i = 0; i < layers.size(); ++i) {
                Image tmp = layers[i].content;
                filter.apply(tmp);
                layers[i].content = tmp;
            }
        } else {
            // Effects: apply to active layer and gate by selection
            applySelectionGate(previewLayer, beforeLayer);
            layers[idx].content = previewLayer;
        }

        // Build preview composite
        compositeLayers();

        // Restore layers to original; keep currentImage as the preview
        layers.swap(saved);
    }

    bool undo() {
        if (undoHistory.empty() || layersUndoHistory.empty()) return false;
        // Move current to redo
        pushRedo();
        // Restore last undo snapshot
        currentImage = undoHistory.back();
        undoHistory.pop_back();
        layers = layersUndoHistory.back();
        layersUndoHistory.pop_back();
        // Rebuild composite to ensure consistency
        compositeLayers();
        return true;
    }

    bool redo() {
        if (redoHistory.empty() || layersRedoHistory.empty()) return false;
        pushUndo();
        currentImage = redoHistory.back();
        redoHistory.pop_back();
        layers = layersRedoHistory.back();
        layersRedoHistory.pop_back();
        compositeLayers();
        return true;
    }

    // Expose read-only accessors for GUI visual history rendering
    const std::vector<Image>& getUndoHistory() const { return undoHistory; }
    const std::vector<Image>& getRedoHistory() const { return redoHistory; }
};