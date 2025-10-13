#pragma once

#include "FilterPreviewSystem.h"
#include "FilterDefs.h"
#include "imgui.h"
#include <memory>
#include <functional>

class FilterPreviewPanel {
private:
    std::unique_ptr<FilterPreviewSystem> previewSystem;
    bool isVisible;
    bool autoGeneratePreviews;
    ImVec2 panelSize;
    int itemsPerRow;
    float itemSpacing;
    
    // UI state
    FilterType selectedFilter;
    bool showFilterNames;
    bool showLoadingIndicators;
    
    // Callback for filter selection
    std::function<void(FilterType)> onFilterSelected;
    
public:
    FilterPreviewPanel();
    ~FilterPreviewPanel();
    
    // Main interface
    void updateImage(const Image& image);
    void render();
    void setVisible(bool visible) { isVisible = visible; }
    bool getVisible() const { return isVisible; }
    
    // Configuration
    void setAutoGenerate(bool autoGen) { autoGeneratePreviews = autoGen; }
    void setPanelSize(ImVec2 size) { panelSize = size; }
    void setItemsPerRow(int items) { itemsPerRow = items; }
    void setItemSpacing(float spacing) { itemSpacing = spacing; }
    
    // Filter selection
    FilterType getSelectedFilter() const { return selectedFilter; }
    void setSelectedFilter(FilterType filter) { selectedFilter = filter; }
    
    // UI options
    void setShowFilterNames(bool show) { showFilterNames = show; }
    void setShowLoadingIndicators(bool show) { showLoadingIndicators = show; }
    
    // Performance
    void generateAllPreviews();
    bool isAnyPreviewGenerating() const;
    void clearPreviews();
    
    // Callback management
    void setOnFilterSelected(std::function<void(FilterType)> callback) { onFilterSelected = callback; }
    
private:
    void renderPreviewGrid();
    void renderPreviewItem(FilterType filterType, ImVec2 pos, ImVec2 size);
    void renderLoadingIndicator(ImVec2 pos, ImVec2 size);
    void renderFilterName(FilterType filterType, ImVec2 pos);
    ImVec2 calculateItemSize() const;
    ImVec2 calculateGridSize() const;
};