#include "LayerManager.h"
#include "../filters/Filter.h"
#include "SelectionMask.h"
#include <algorithm>
#include <stdexcept>

LayerManager::LayerManager() : activeLayerIndex(-1), nextZIndex(0) {
}

Layer* LayerManager::getLayerByIndex(int index) {
    if (index < 0 || index >= (int)layers.size()) {
        return nullptr;
    }
    return layers[index].get();
}

const Layer* LayerManager::getLayerByIndex(int index) const {
    if (index < 0 || index >= (int)layers.size()) {
        return nullptr;
    }
    return layers[index].get();
}

int LayerManager::addLayer(const std::string& name, int width, int height) {
    auto newLayer = std::make_unique<Layer>(name, width, height);
    newLayer->setZIndex(nextZIndex++);
    layers.push_back(std::move(newLayer));
    
    if (activeLayerIndex == -1) {
        activeLayerIndex = layers.size() - 1;
    }
    
    return layers.size() - 1;
}

int LayerManager::addLayer(const std::string& name, const Image& image) {
    auto newLayer = std::make_unique<Layer>(name, image);
    newLayer->setZIndex(nextZIndex++);
    layers.push_back(std::move(newLayer));
    
    if (activeLayerIndex == -1) {
        activeLayerIndex = layers.size() - 1;
    }
    
    return layers.size() - 1;
}

bool LayerManager::removeLayer(int index) {
    if (index < 0 || index >= (int)layers.size()) {
        return false;
    }
    
    layers.erase(layers.begin() + index);
    
    // Adjust active layer index
    if (activeLayerIndex >= (int)layers.size()) {
        activeLayerIndex = layers.size() - 1;
    }
    if (layers.empty()) {
        activeLayerIndex = -1;
    }
    
    return true;
}

bool LayerManager::duplicateLayer(int index) {
    if (index < 0 || index >= (int)layers.size()) {
        return false;
    }
    
    const Layer* sourceLayer = layers[index].get();
    std::string newName = sourceLayer->getName() + " Copy";
    
    auto newLayer = std::make_unique<Layer>(*sourceLayer);
    newLayer->setName(newName);
    newLayer->setZIndex(nextZIndex++);
    
    layers.insert(layers.begin() + index + 1, std::move(newLayer));
    
    return true;
}

bool LayerManager::moveLayer(int fromIndex, int toIndex) {
    if (fromIndex < 0 || fromIndex >= (int)layers.size() ||
        toIndex < 0 || toIndex >= (int)layers.size() ||
        fromIndex == toIndex) {
        return false;
    }
    
    // Move the layer
    auto layer = std::move(layers[fromIndex]);
    layers.erase(layers.begin() + fromIndex);
    layers.insert(layers.begin() + toIndex, std::move(layer));
    
    // Adjust active layer index
    if (activeLayerIndex == fromIndex) {
        activeLayerIndex = toIndex;
    } else if (fromIndex < activeLayerIndex && toIndex >= activeLayerIndex) {
        activeLayerIndex--;
    } else if (fromIndex > activeLayerIndex && toIndex <= activeLayerIndex) {
        activeLayerIndex++;
    }
    
    return true;
}

bool LayerManager::setActiveLayer(int index) {
    if (index < 0 || index >= (int)layers.size()) {
        return false;
    }
    activeLayerIndex = index;
    return true;
}

Layer* LayerManager::getActiveLayer() {
    return getLayerByIndex(activeLayerIndex);
}

const Layer* LayerManager::getActiveLayer() const {
    return getLayerByIndex(activeLayerIndex);
}

Layer* LayerManager::getLayer(int index) {
    return getLayerByIndex(index);
}

const Layer* LayerManager::getLayer(int index) const {
    return getLayerByIndex(index);
}

bool LayerManager::renameLayer(int index, const std::string& newName) {
    Layer* layer = getLayerByIndex(index);
    if (!layer) return false;
    
    layer->setName(newName);
    return true;
}

bool LayerManager::setLayerOpacity(int index, float opacity) {
    Layer* layer = getLayerByIndex(index);
    if (!layer) return false;
    
    layer->setOpacity(opacity);
    return true;
}

bool LayerManager::setLayerVisibility(int index, bool visible) {
    Layer* layer = getLayerByIndex(index);
    if (!layer) return false;
    
    layer->setVisible(visible);
    return true;
}

bool LayerManager::setLayerLocked(int index, bool locked) {
    Layer* layer = getLayerByIndex(index);
    if (!layer) return false;
    
    layer->setLocked(locked);
    return true;
}

bool LayerManager::setLayerBlendMode(int index, BlendMode mode) {
    Layer* layer = getLayerByIndex(index);
    if (!layer) return false;
    
    layer->setBlendMode(mode);
    return true;
}

Image LayerManager::composeLayers() const {
    if (layers.empty()) {
        return Image(1, 1); // Return empty image
    }
    
    // Sort layers by z-index (bottom to top)
    std::vector<int> sortedIndices;
    for (int i = 0; i < (int)layers.size(); i++) {
        sortedIndices.push_back(i);
    }
    
    std::sort(sortedIndices.begin(), sortedIndices.end(), [this](int a, int b) {
        return layers[a]->getZIndex() < layers[b]->getZIndex();
    });
    
    // Start with the bottom layer
    Image result = layers[sortedIndices[0]]->getImage();
    
    // Blend each subsequent layer
    for (size_t i = 1; i < sortedIndices.size(); i++) {
        const Layer* layer = layers[sortedIndices[i]].get();
        if (layer->isVisible()) {
            result = Layer::blendLayers(Layer("temp", result), *layer);
        }
    }
    
    return result;
}

Image LayerManager::composeLayersUpTo(int maxIndex) const {
    if (layers.empty() || maxIndex < 0) {
        return Image(1, 1);
    }
    
    int actualMaxIndex = std::min(maxIndex, (int)layers.size() - 1);
    
    // Sort layers by z-index up to maxIndex
    std::vector<int> sortedIndices;
    for (int i = 0; i <= actualMaxIndex; i++) {
        sortedIndices.push_back(i);
    }
    
    std::sort(sortedIndices.begin(), sortedIndices.end(), [this](int a, int b) {
        return layers[a]->getZIndex() < layers[b]->getZIndex();
    });
    
    // Start with the bottom layer
    Image result = layers[sortedIndices[0]]->getImage();
    
    // Blend each subsequent layer
    for (size_t i = 1; i < sortedIndices.size(); i++) {
        const Layer* layer = layers[sortedIndices[i]].get();
        if (layer->isVisible()) {
            result = Layer::blendLayers(Layer("temp", result), *layer);
        }
    }
    
    return result;
}

std::vector<std::string> LayerManager::getLayerNames() const {
    std::vector<std::string> names;
    for (const auto& layer : layers) {
        names.push_back(layer->getName());
    }
    return names;
}

void LayerManager::clear() {
    layers.clear();
    activeLayerIndex = -1;
    nextZIndex = 0;
}

LayerManager LayerManager::clone() const {
    LayerManager newManager;
    newManager.nextZIndex = nextZIndex;
    
    for (const auto& layer : layers) {
        auto newLayer = std::make_unique<Layer>(*layer);
        newManager.layers.push_back(std::move(newLayer));
    }
    
    newManager.activeLayerIndex = activeLayerIndex;
    return newManager;
}

bool LayerManager::applyFilterToLayer(int layerIndex, Filter& filter) {
    Layer* layer = getLayerByIndex(layerIndex);
    if (!layer || layer->isLocked()) {
        return false;
    }
    
    filter.apply(layer->getImage());
    return true;
}

bool LayerManager::applyFilterToActiveLayer(Filter& filter) {
    return applyFilterToLayer(activeLayerIndex, filter);
}

bool LayerManager::applyFilterToSelection(int layerIndex, Filter& filter, const SelectionMask& selection) {
    Layer* layer = getLayerByIndex(layerIndex);
    if (!layer || layer->isLocked()) {
        return false;
    }
    
    // Apply filter only to selected pixels
    Image& layerImage = layer->getImage();
    Image tempImage = layerImage;
    
    // Apply filter to temporary image
    filter.apply(tempImage);
    
    // Copy only selected pixels back to original
    for (int y = 0; y < layerImage.height && y < selection.getHeight(); y++) {
        for (int x = 0; x < layerImage.width && x < selection.getWidth(); x++) {
            if (selection.isSelected(x, y)) {
                for (int c = 0; c < 3; c++) {
                    layerImage.setPixel(x, y, c, tempImage.getPixel(x, y, c));
                }
            }
        }
    }
    
    return true;
}

bool LayerManager::applyFilterToActiveSelection(Filter& filter, const SelectionMask& selection) {
    return applyFilterToSelection(activeLayerIndex, filter, selection);
}