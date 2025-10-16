#pragma once

#include "Layer.h"
#include <vector>
#include <memory>
#include <string>

class LayerManager {
private:
    std::vector<std::unique_ptr<Layer>> layers;
    int activeLayerIndex;
    int nextZIndex;
    
    // Helper to find layer by index
    Layer* getLayerByIndex(int index);
    const Layer* getLayerByIndex(int index) const;

public:
    LayerManager();
    ~LayerManager() = default;

    // Layer management
    int addLayer(const std::string& name, int width, int height);
    int addLayer(const std::string& name, const Image& image);
    bool removeLayer(int index);
    bool duplicateLayer(int index);
    bool moveLayer(int fromIndex, int toIndex);
    
    // Layer properties
    bool setActiveLayer(int index);
    int getActiveLayerIndex() const { return activeLayerIndex; }
    Layer* getActiveLayer();
    const Layer* getActiveLayer() const;
    Layer* getLayer(int index);
    const Layer* getLayer(int index) const;
    
    // Layer operations
    bool renameLayer(int index, const std::string& newName);
    bool setLayerOpacity(int index, float opacity);
    bool setLayerVisibility(int index, bool visible);
    bool setLayerLocked(int index, bool locked);
    bool setLayerBlendMode(int index, BlendMode mode);
    
    // Layer composition
    Image composeLayers() const;
    Image composeLayersUpTo(int maxIndex) const;
    
    // Layer information
    int getLayerCount() const { return layers.size(); }
    std::vector<std::string> getLayerNames() const;
    
    // History support
    void clear();
    LayerManager clone() const;
    
    // Layer effects and filters
    bool applyFilterToLayer(int layerIndex, class Filter& filter);
    bool applyFilterToActiveLayer(class Filter& filter);
    
    // Selection support
    bool applyFilterToSelection(int layerIndex, class Filter& filter, const class SelectionMask& selection);
    bool applyFilterToActiveSelection(class Filter& filter, const class SelectionMask& selection);
};