#pragma once

#include <GL/gl.h>
#include <vector>
#include <string>
#include <algorithm>

#include "../../external/Image_Class.h"
#include "Layer.h"
#include "Selection.h"
#include "../filters/Filter.h"

// Layered image processing with selection and undo/redo management
class ImageProcessor {
private:
    // GL texture id is managed by GUI; we just expose it for compatibility
    GLuint currentTextureID = 0;

    // Layers and active selection
    std::vector<Layer> m_layers;
    int m_activeLayerIndex = -1;
    Selection m_currentSelection;

    // Composite cache
    mutable Image m_compositeImage;
    mutable bool m_compositeDirty = true;

    // Undo/redo state snapshots (layers) and thumbnails (for GUI)
    std::vector<std::vector<Layer>> m_layersUndoHistory;
    std::vector<std::vector<Layer>> m_layersRedoHistory;
    std::vector<Image> undoHistory; // thumbnails for GUI history view
    std::vector<Image> redoHistory; // thumbnails for GUI history view
    int historySize;

    // --- Helpers ---
    void pushUndo() {
        if ((int)m_layersUndoHistory.size() >= historySize) {
            m_layersUndoHistory.erase(m_layersUndoHistory.begin());
            if (!undoHistory.empty()) undoHistory.erase(undoHistory.begin());
        }
        m_layersUndoHistory.push_back(m_layers);
        // store composite snapshot for thumbnails
        undoHistory.push_back(getCompositeImage());
    }

    void pushRedo() {
        if ((int)m_layersRedoHistory.size() >= historySize) {
            m_layersRedoHistory.erase(m_layersRedoHistory.begin());
            if (!redoHistory.empty()) redoHistory.erase(redoHistory.begin());
        }
        m_layersRedoHistory.push_back(m_layers);
        redoHistory.push_back(getCompositeImage());
    }

    static unsigned char blendChannel(unsigned char dst, unsigned char src, float alpha) {
        float d = (float)dst, s = (float)src;
        float out = d * (1.0f - alpha) + s * (alpha);
        if (out < 0.0f) out = 0.0f; if (out > 255.0f) out = 255.0f;
        return (unsigned char)(out + 0.5f);
    }

public:
    ImageProcessor(int hSize = 10) : historySize(hSize) {}

    // --- Layer management ---
    void addLayer(const Image &initialImage) {
        Layer L(initialImage, std::string("Layer ") + std::to_string((int)m_layers.size() + 1));
        m_layers.push_back(L);
        setActiveLayer((int)m_layers.size() - 1);
        invalidateComposite();
    }

    void deleteLayer(int index) {
        if (index < 0 || index >= (int)m_layers.size()) return;
        m_layers.erase(m_layers.begin() + index);
        if (m_layers.empty()) {
            m_activeLayerIndex = -1;
        } else {
            m_activeLayerIndex = std::clamp(index - 1, 0, (int)m_layers.size() - 1);
            for (int i = 0; i < (int)m_layers.size(); ++i) m_layers[i].isSelected = (i == m_activeLayerIndex);
        }
        invalidateComposite();
    }

    void moveLayer(int fromIndex, int toIndex) {
        if (fromIndex < 0 || fromIndex >= (int)m_layers.size()) return;
        if (toIndex < 0 || toIndex >= (int)m_layers.size()) return;
        if (fromIndex == toIndex) return;
        Layer temp = m_layers[fromIndex];
        m_layers.erase(m_layers.begin() + fromIndex);
        m_layers.insert(m_layers.begin() + toIndex, temp);
        if (m_activeLayerIndex == fromIndex) m_activeLayerIndex = toIndex;
        invalidateComposite();
    }

    void setActiveLayer(int index) {
        if (index < 0 || index >= (int)m_layers.size()) { m_activeLayerIndex = -1; return; }
        m_activeLayerIndex = index;
        for (int i = 0; i < (int)m_layers.size(); ++i) m_layers[i].isSelected = (i == index);
    }

    int getActiveLayerIndex() const { return m_activeLayerIndex; }

    const std::vector<Layer>& getLayers() const { return m_layers; }
    std::vector<Layer>& getLayersMutable() { return m_layers; }

    Selection& selection() { return m_currentSelection; }
    const Selection& selection() const { return m_currentSelection; }

    void invalidateComposite() { m_compositeDirty = true; }

    // --- I/O ---
    void loadImage(const std::string &filename) {
        Image newImage(filename);
        m_layers.clear(); m_layersUndoHistory.clear(); m_layersRedoHistory.clear();
        undoHistory.clear(); redoHistory.clear();
        addLayer(newImage);
        m_currentSelection.clear();
        invalidateComposite();
    }

    bool saveImage(const std::string &filename) {
        const Image &compositeRef = getCompositeImage();
        if (compositeRef.width <= 0 || compositeRef.height <= 0) return false;
        Image composite = compositeRef; // make non-const copy since saveImage is non-const
        return composite.saveImage(filename);
    }

    // --- Filters ---
    void applyFilter(Filter &filter) {
        if (m_activeLayerIndex < 0 || m_activeLayerIndex >= (int)m_layers.size()) return;
        pushUndo();
        m_layersRedoHistory.clear();
        redoHistory.clear();

        // Ensure mask matches composite size
        const Image &base = getCompositeImage();
        m_currentSelection.ensureMaskSize(base.width, base.height);

        Image &target = m_layers[m_activeLayerIndex].activeImage;
        Image result = filter.apply(target, m_currentSelection.mask);
        target = result; // commit
        invalidateComposite();
    }

    // Apply without history (e.g., live previews)
    void applyFilterNoHistory(Filter &filter) {
        if (m_activeLayerIndex < 0 || m_activeLayerIndex >= (int)m_layers.size()) return;
        const Image &base = getCompositeImage();
        m_currentSelection.ensureMaskSize(base.width, base.height);
        Image &target = m_layers[m_activeLayerIndex].activeImage;
        Image result = filter.apply(target, m_currentSelection.mask);
        target = result;
        invalidateComposite();
    }

    // --- Undo/Redo ---
    bool undo() {
        if (m_layersUndoHistory.empty()) return false;
        // Save current state to redo
        if ((int)m_layersRedoHistory.size() >= historySize) {
            m_layersRedoHistory.erase(m_layersRedoHistory.begin());
            if (!redoHistory.empty()) redoHistory.erase(redoHistory.begin());
        }
        m_layersRedoHistory.push_back(m_layers);
        redoHistory.push_back(getCompositeImage());
        // Restore previous
        m_layers = m_layersUndoHistory.back();
        m_layersUndoHistory.pop_back();
        invalidateComposite();
        return true;
    }

    bool redo() {
        if (m_layersRedoHistory.empty()) return false;
        // Save current to undo
        if ((int)m_layersUndoHistory.size() >= historySize) {
            m_layersUndoHistory.erase(m_layersUndoHistory.begin());
            if (!undoHistory.empty()) undoHistory.erase(undoHistory.begin());
        }
        m_layersUndoHistory.push_back(m_layers);
        undoHistory.push_back(getCompositeImage());
        // Restore from redo
        m_layers = m_layersRedoHistory.back();
        m_layersRedoHistory.pop_back();
        invalidateComposite();
        return true;
    }

    // --- Accessors ---
    const Image& getCompositeImage() const {
        if (!m_compositeDirty) return m_compositeImage;
        if (m_layers.empty()) { m_compositeImage = Image(); m_compositeDirty = false; return m_compositeImage; }

        // Find first visible layer as base
        int baseIdx = -1;
        for (int i = 0; i < (int)m_layers.size(); ++i) {
            if (m_layers[i].isVisible && m_layers[i].activeImage.width > 0 && m_layers[i].activeImage.height > 0) { baseIdx = i; break; }
        }
        if (baseIdx == -1) { m_compositeImage = Image(); m_compositeDirty = false; return m_compositeImage; }

        m_compositeImage = m_layers[baseIdx].activeImage; // copy
        for (int i = baseIdx + 1; i < (int)m_layers.size(); ++i) {
            const Layer &L = m_layers[i];
            if (!L.isVisible) continue;
            const Image &src = L.activeImage;
            if (src.width != m_compositeImage.width || src.height != m_compositeImage.height) continue; // skip mismatched
            float alpha = std::clamp(L.opacity, 0.0f, 1.0f);
            if (alpha <= 0.0f) continue;
            for (int y = 0; y < m_compositeImage.height; ++y) {
                for (int x = 0; x < m_compositeImage.width; ++x) {
                    for (int c = 0; c < 3; ++c) {
                        unsigned char d = m_compositeImage(x, y, c);
                        unsigned char s = src(x, y, c);
                        m_compositeImage(x, y, c) = blendChannel(d, s, alpha);
                    }
                }
            }
        }
        m_compositeDirty = false;
        return m_compositeImage;
    }

    // Backward-compat name for GUI: now returns composite
    const Image& getCurrentImage() const { return getCompositeImage(); }

    // Set or replace image in active layer (used by batch/presets)
    void setImage(Image &newImage) {
        if (m_activeLayerIndex < 0) {
            addLayer(newImage);
        } else {
            m_layers[m_activeLayerIndex].activeImage = newImage;
            invalidateComposite();
        }
    }

    const GLuint& getTextureID() const { return currentTextureID; }

    // Expose read-only accessors for GUI visual history rendering (thumbnails only)
    const std::vector<Image>& getUndoHistory() const { return undoHistory; }
    const std::vector<Image>& getRedoHistory() const { return redoHistory; }
};
