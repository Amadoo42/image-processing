#include "FilterPreviewPanel.h"
#include <iostream>
#include <cmath>

FilterPreviewPanel::FilterPreviewPanel() 
    : previewSystem(std::make_unique<FilterPreviewSystem>())
    , isVisible(true)
    , autoGeneratePreviews(true)
    , panelSize(ImVec2(500, 400))
    , itemsPerRow(1)
    , itemSpacing(10.0f)
    , selectedFilter(FilterType::None)
    , showFilterNames(true)
    , showLoadingIndicators(true) {
}

FilterPreviewPanel::~FilterPreviewPanel() = default;

void FilterPreviewPanel::updateImage(const Image& image) {
    if (previewSystem) {
        previewSystem->updateImage(image);
        
        if (autoGeneratePreviews) {
            previewSystem->generateAllPreviews();
        }
    }
}

void FilterPreviewPanel::render() {
    if (!isVisible) {
        return;
    }
    
    ImGui::BeginChild("Filter Preview Panel", panelSize, true);
    
    ImGui::Text("Filter Previews");
    ImGui::Separator();
    
    // Show loading status
    if (isAnyPreviewGenerating()) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Generating previews...");
    }
    
    // Render preview grid
    renderPreviewGrid();
    
    ImGui::EndChild();
}

void FilterPreviewPanel::renderPreviewGrid() {
    if (!previewSystem) {
        return;
    }
    
    ImVec2 itemSize = calculateItemSize();
    ImVec2 gridSize = calculateGridSize();
    
    // Calculate starting position
    ImVec2 startPos = ImGui::GetCursorPos();
    ImVec2 currentPos = startPos;
    
    // Get available filters (visual filters only)
    std::vector<FilterType> filterTypes = {
        FilterType::Grayscale, FilterType::Invert, FilterType::BlackAndWhite,
        FilterType::Blur, FilterType::Contrast, FilterType::Saturation,
        FilterType::Brightness, FilterType::HorizontalFlip, FilterType::VerticalFlip,
        FilterType::Outline, FilterType::Purple, FilterType::Infrared, 
        FilterType::OilPainting, FilterType::Retro, FilterType::Vignette, FilterType::Warmth
        // Note: Rotate, Crop, Resize, Skew, Merge, Frame, Wave filters are skipped as they require parameters or are not suitable for preview
    };
    
    int itemCount = 0;
    
    for (FilterType filterType : filterTypes) {
        // Check if we need to start a new row
        if (itemCount > 0 && itemCount % itemsPerRow == 0) {
            currentPos.x = startPos.x;
            currentPos.y += itemSize.y + itemSpacing;
        }
        
        // Render the preview item
        renderPreviewItem(filterType, currentPos, itemSize);
        
        // Move to next position
        if (itemCount % itemsPerRow != itemsPerRow - 1) {
            currentPos.x += itemSize.x + itemSpacing;
        }
        
        itemCount++;
    }
    
    // Update cursor position for next UI elements
    ImGui::SetCursorPos(ImVec2(startPos.x, currentPos.y + itemSize.y + itemSpacing));
}

void FilterPreviewPanel::renderPreviewItem(FilterType filterType, ImVec2 pos, ImVec2 size) {
    ImGui::SetCursorPos(pos);
    
    // Create unique ID for this filter
    ImGui::PushID(static_cast<int>(filterType));
    
    // Check if preview is ready
    bool isReady = previewSystem->isPreviewReady(filterType);
    bool isGenerating = previewSystem->isAnyPreviewGenerating();
    
    if (isReady) {
        // Get the preview
        auto preview = previewSystem->getPreview(filterType);
        if (preview && preview->textureID != 0) {
            // Create image button
            if (ImGui::ImageButton("##preview", (void*)(intptr_t)preview->textureID, size)) {
                selectedFilter = filterType;
                if (onFilterSelected) {
                    onFilterSelected(filterType);
                }
                std::cout << "Selected filter: " << filterTypeName(filterType) << std::endl;
            }
            
            // Show selection highlight
            if (selectedFilter == filterType) {
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                drawList->AddRect(min, max, ImGui::GetColorU32(ImGuiCol_ButtonActive), 0.0f, 0, 2.0f);
            }
        } else {
            // Fallback button
            ImGui::Button("Preview", size);
        }
    } else if (isGenerating && showLoadingIndicators) {
        // Show loading indicator
        renderLoadingIndicator(pos, size);
    } else {
        // Show placeholder
        ImGui::Button("No Preview", size);
    }
    
    // Show filter name
    if (showFilterNames) {
        renderFilterName(filterType, ImVec2(pos.x, pos.y + size.y + 2));
    }
    
    ImGui::PopID();
}

void FilterPreviewPanel::renderLoadingIndicator(ImVec2 pos, ImVec2 size) {
    ImGui::SetCursorPos(pos);
    
    // Create a simple loading animation
    static float loadingTime = 0.0f;
    loadingTime += ImGui::GetIO().DeltaTime;
    
    float alpha = (sin(loadingTime * 3.0f) + 1.0f) * 0.5f;
    ImVec4 color = ImVec4(1.0f, 1.0f, 0.0f, alpha);
    
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::Button("Loading...", size);
    ImGui::PopStyleColor();
}

void FilterPreviewPanel::renderFilterName(FilterType filterType, ImVec2 pos) {
    ImGui::SetCursorPos(pos);
    
    const char* name = filterTypeName(filterType);
    ImVec2 textSize = ImGui::CalcTextSize(name);
    
    // Center the text
    ImVec2 centerPos = ImVec2(
        pos.x + (calculateItemSize().x - textSize.x) * 0.5f,
        pos.y
    );
    
    ImGui::SetCursorPos(centerPos);
    ImGui::Text("%s", name);
}

ImVec2 FilterPreviewPanel::calculateItemSize() const {
    float availableWidth = panelSize.x - (itemsPerRow + 1) * itemSpacing;
    float itemWidth = availableWidth / itemsPerRow;
    float itemHeight = itemWidth * 0.75f; // 4:3 aspect ratio
    
    return ImVec2(itemWidth, itemHeight);
}

ImVec2 FilterPreviewPanel::calculateGridSize() const {
    ImVec2 itemSize = calculateItemSize();
    
    // Calculate number of rows needed
    std::vector<FilterType> filterTypes = {
        FilterType::Grayscale, FilterType::Invert, FilterType::BlackAndWhite,
        FilterType::Blur, FilterType::Contrast, FilterType::Saturation,
        FilterType::Brightness, FilterType::HorizontalFlip, FilterType::VerticalFlip,
        FilterType::Outline, FilterType::Purple, FilterType::Infrared, 
        FilterType::OilPainting, FilterType::Retro, FilterType::Vignette, FilterType::Warmth
        // Note: Rotate, Crop, Resize, Skew, Merge, Frame, Wave filters are skipped as they require parameters or are not suitable for preview
    };
    
    int numItems = static_cast<int>(filterTypes.size());
    int numRows = (numItems + itemsPerRow - 1) / itemsPerRow;
    
    float totalWidth = itemsPerRow * itemSize.x + (itemsPerRow - 1) * itemSpacing;
    float totalHeight = numRows * itemSize.y + (numRows - 1) * itemSpacing;
    
    return ImVec2(totalWidth, totalHeight);
}

void FilterPreviewPanel::generateAllPreviews() {
    if (previewSystem) {
        previewSystem->generateAllPreviews();
    }
}

bool FilterPreviewPanel::isAnyPreviewGenerating() const {
    return previewSystem ? previewSystem->isAnyPreviewGenerating() : false;
}

void FilterPreviewPanel::clearPreviews() {
    if (previewSystem) {
        previewSystem->clearPreviews();
    }
}