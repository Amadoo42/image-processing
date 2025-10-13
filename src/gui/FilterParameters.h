#pragma once

#include "../core/ImageProcessor.h"
#include "MemoryOperation.h"
#include "LoadTexture.h"
#include "../filters/GrayscaleFilter.h"
#include "../filters/InvertFilter.h"
#include "../filters/BlurFilter.h"
#include "../filters/BlackAndWhiteFilter.h"
#include "../filters/CropFilter.h"
#include "../filters/ResizeFilter.h"
#include "../filters/DarkenFilter.h"
#include "../filters/LightenFilter.h"
#include "../filters/FrameFilter.h"
#include "../filters/HorizontalFlipFilter.h"
#include "../filters/VerticalFlipFilter.h"
#include "../filters/MergeFilter.h"
#include "../filters/RotateFilter.h"
#include "../filters/OutlineFilter.h"
#include "../filters/PurpleFilter.h"
#include "../filters/InfraredFilter.h"
#include "../filters/WaveFilter.h"
#include "../filters/OilPaintingFilter.h"
#include "../filters/ContrastFilter.h"
#include "../filters/RetroFilter.h"
#include "../filters/SaturationFilter.h"
#include "../filters/SkewFilter.h"
#include "../filters/VigentteFilter.h"
#include "../filters/WarmthFilter.h"

inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x + b.x, a.y + b.y); }
inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) { return ImVec2(a.x - b.x, a.y - b.y); }
inline ImVec2 operator*(const ImVec2& a, float b) { return ImVec2(a.x * b, a.y * b); }

class FilterParameters {
public:
    FilterParameters(ImageProcessor &processor) : processor(processor) {};

    void applyGrayscale(bool &show, bool &textureNeedsUpdate) {
        if(show){
            GrayscaleFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Grayscale Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyInvert(bool &show, bool &textureNeedsUpdate) {
        if(show){
            InvertFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Invert Filter\n";
            
            show = false;
            textureNeedsUpdate = true;       
        }
    }

    void applyBlur(bool &show, bool &textureNeedsUpdate) {
        static int currentItem = 0;
        const char* items[] = {"Low", "Medium", "High"};
        int values1[] = {3, 7, 15};
        double values2[] = {0.8, 1.6, 3.0};

        static Image originalImage;
        static bool init = false;
        if(show){
            ImGui::Begin("Oil Painting Parameters", &show); // Wtf???

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            ImGui::Text("Intenisty:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::Combo("Intenisty", &currentItem, items, IM_ARRAYSIZE(items))) changed = true;

            if(changed){
                processor.setImage(originalImage);
                BlurFilter filter(values1[currentItem], values2[currentItem]); 
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            ImGui::Separator();

            if(ImGui::Button("Apply")){
                BlurFilter filter(values1[currentItem], values2[currentItem]); 
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }
    void applyBlackAndWhite(bool &show, bool &textureNeedsUpdate) {
        if(show){
            BlackAndWhiteFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Black and White Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }

    void applyCrop(bool &show, bool &textureNeedsUpdate) {
        static int posX = 50, posY = 50, newWidth = 200, newHeight = 200;
        static Image originalImage;
        static bool init = false;
        static GLuint textureID = 0;

        if (!show) { 
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
                textureID = 0;
            }
            init = false; 
            return; 
        }

        ImGuiIO& io = ImGui::GetIO();

        if (!init) {
            ImGui::OpenPopup("Crop Overlay");
            originalImage = processor.getCurrentImage();
            posX = 0;
            posY = 0;
            newWidth = originalImage.width;
            newHeight = originalImage.height;

            textureID = loadTexture(originalImage);
            init = true;
        }

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse;

        if (!ImGui::BeginPopupModal("Crop Overlay", &show, flags)) {
            return;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        draw->AddRectFilled(ImVec2(0, 0), io.DisplaySize, IM_COL32(0, 0, 0, 220));

        bool changed = false;

        float displayedWidth = (float)originalImage.width;
        float displayedHeight = (float)originalImage.height;

        float scale = 1.0f;
        if (displayedWidth > io.DisplaySize.x) scale = io.DisplaySize.x / displayedWidth;
        if (displayedHeight * scale > io.DisplaySize.y) scale = io.DisplaySize.y / displayedHeight;
        displayedWidth *= scale;
        displayedHeight *= scale;

        ImVec2 imagePos(
            (io.DisplaySize.x - displayedWidth) * 0.5f,
            (io.DisplaySize.y - displayedHeight) * 0.5f
        );

        ImVec2 imageMax(imagePos.x + displayedWidth, imagePos.y + displayedHeight);

        draw->AddImage((void*)(intptr_t)textureID, imagePos, imageMax, ImVec2(0, 0), ImVec2(1, 1));

        float scaleX = displayedWidth / (float)originalImage.width;
        float scaleY = displayedHeight / (float)originalImage.height;

        ImVec2 cropMin(imagePos.x + posX * scaleX, imagePos.y + posY * scaleY);
        ImVec2 cropMax(cropMin.x + newWidth * scaleX, cropMin.y + newHeight * scaleY);

        draw->AddRectFilled(imagePos, ImVec2(imageMax.x, cropMin.y), IM_COL32(0, 0, 0, 160));
        draw->AddRectFilled(ImVec2(imagePos.x, cropMax.y), imageMax, IM_COL32(0, 0, 0, 160));
        draw->AddRectFilled(ImVec2(imagePos.x, cropMin.y), ImVec2(cropMin.x, cropMax.y), IM_COL32(0, 0, 0, 160));
        draw->AddRectFilled(ImVec2(cropMax.x, cropMin.y), ImVec2(imageMax.x, cropMax.y), IM_COL32(0, 0, 0, 160));
        draw->AddRect(cropMin, cropMax, IM_COL32(255, 255, 255, 255), 0, 0, 2.0f);

        ImGui::SetCursorPos(ImVec2(20, 20));
        ImGui::BeginChild("Crop Controls", ImVec2(250, 160), true);
        ImGui::Text("Crop Parameters");
        ImGui::Separator();
        if (ImGui::InputInt("X", &posX)) changed = true;
        if (ImGui::InputInt("Y", &posY)) changed = true;
        if (ImGui::InputInt("Width", &newWidth)) changed = true;
        if (ImGui::InputInt("Height", &newHeight)) changed = true;

        if (ImGui::Button("Apply")) {
            processor.setImage(originalImage);
            CropFilter f(posX, posY, newWidth, newHeight);
            processor.applyFilter(f);
            textureNeedsUpdate = true;
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
                textureID = 0;
            }
            ImGui::CloseCurrentPopup();
            show = false;
            init = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            processor.setImage(originalImage);
            textureNeedsUpdate = true;
            if (textureID != 0) {
                glDeleteTextures(1, &textureID);
                textureID = 0;
            }
            ImGui::CloseCurrentPopup();
            show = false;
            init = false;
        }
        ImGui::EndChild();

        static bool dragging = false;
        static bool resizing = false;
        static int resizeCorner = -1;
        static ImVec2 dragStart;
        static int startX, startY, startW, startH;
        ImVec2 mouse = io.MousePos;

        const float cornerSize = 15.0f;

        ImVec2 corners[4] = {
            cropMin,                                    
            ImVec2(cropMax.x, cropMin.y),          
            cropMax,                                    
            ImVec2(cropMin.x, cropMax.y)            
        };

        for (int i = 0; i < 4; i++) {
            draw->AddCircleFilled(corners[i], 6.0f, IM_COL32(255, 255, 255, 255));
            draw->AddCircle(corners[i], 6.0f, IM_COL32(0, 0, 0, 255), 0, 2.0f);
        }

        bool insideCrop = mouse.x >= cropMin.x && mouse.x <= cropMax.x && mouse.y >= cropMin.y && mouse.y <= cropMax.y;
        bool overCorner = false;
        int hoveredCorner = -1;

        if (!dragging && !resizing) {
            for (int i = 0; i < 4; i++) {
                float dist = sqrtf((mouse.x - corners[i].x) * (mouse.x - corners[i].x) + 
                                (mouse.y - corners[i].y) * (mouse.y - corners[i].y));
                if (dist <= cornerSize) {
                    overCorner = true;
                    hoveredCorner = i;
                    break;
                }
            }
        }

        if (overCorner && ImGui::IsMouseClicked(0)) {
            resizing = true;
            resizeCorner = hoveredCorner;
            dragStart = mouse;
            startX = posX;
            startY = posY;
            startW = newWidth;
            startH = newHeight;
        }

        else if (insideCrop && !overCorner && ImGui::IsMouseClicked(0)) {
            dragging = true;
            dragStart = mouse;
            startX = posX;
            startY = posY;
        }

        if (resizing && ImGui::IsMouseDown(0)) {
            ImVec2 delta = ImVec2(mouse.x - dragStart.x, mouse.y - dragStart.y);
            int dx = (int)(delta.x / scaleX);
            int dy = (int)(delta.y / scaleY);

            switch (resizeCorner) {
                case 0:
                    posX = std::clamp(startX + dx, 0, startX + startW - 1);
                    posY = std::clamp(startY + dy, 0, startY + startH - 1);
                    newWidth = (startX + startW) - posX;
                    newHeight = (startY + startH) - posY;
                    break;
                case 1:
                    posY = std::clamp(startY + dy, 0, startY + startH - 1);
                    newWidth = std::clamp(startW + dx, 1, originalImage.width - startX);
                    newHeight = (startY + startH) - posY;
                    break;
                case 2:
                    newWidth = std::clamp(startW + dx, 1, originalImage.width - startX);
                    newHeight = std::clamp(startH + dy, 1, originalImage.height - startY);
                    break;
                case 3:
                    posX = std::clamp(startX + dx, 0, startX + startW - 1);
                    newWidth = (startX + startW) - posX;
                    newHeight = std::clamp(startH + dy, 1, originalImage.height - startY);
                    break;
            }
            changed = true;
        }

        if (dragging && ImGui::IsMouseDown(0)) {
            ImVec2 delta = ImVec2(mouse.x - dragStart.x, mouse.y - dragStart.y);
            int dx = (int)(delta.x / scaleX);
            int dy = (int)(delta.y / scaleY);
            posX = std::clamp(startX + dx, 0, originalImage.width - newWidth);
            posY = std::clamp(startY + dy, 0, originalImage.height - newHeight);
            changed = true;
        }

        if (!ImGui::IsMouseDown(0)) {
            dragging = false;
            resizing = false;
            resizeCorner = -1;
        }

        newWidth = std::max(1, newWidth);
        newHeight = std::max(1, newHeight);
        posX = std::clamp(posX, 0, std::max(0, originalImage.width - newWidth));
        posY = std::clamp(posY, 0, std::max(0, originalImage.height - newHeight));

        ImGui::EndPopup();
    }


    // #TODO: overlay
    void applyResize(bool &show, bool &textureNeedsUpdate) {
        static int newWidth, newHeight;
        static Image originalImage;
        static bool init = false;
        if(show){
            ImGui::Begin("Resize Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            bool changed = false;
            
            ImGui::Text("Width:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Width", &newWidth, 0, 0))changed = true;
            
            ImGui::Text("Height:");
            ImGui::SameLine();
            if(ImGui::InputInt("##Height", &newHeight, 0, 0))changed = true;

            if(changed){
                processor.setImage(originalImage);
                ResizeFilter filter(newWidth, newHeight);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            
            ImGui::Separator();
            
            if(ImGui::Button("Apply")){
                ResizeFilter filter(newWidth, newHeight);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Resize filter applied with Parameters: " << newWidth << " " << newHeight << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }

    void applyBrightness(bool &show, bool &textureNeedsUpdate) {
        static float factor = 1.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Brightness Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, 0.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                DarkenFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                DarkenFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Brightness filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }

    void applyFrame(bool &show, bool &textureNeedsUpdate) {
        if(show){
            Image frame_image(openFileDialog_Linux());
            FrameFilter filter(frame_image);
            processor.applyFilter(filter);
            std::cout << "Applied Frame Filter\n";
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyHorizontalFlip(bool &show, bool &textureNeedsUpdate) {
        if(show){
            HorizontalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Horizontal Flip Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyVerticalFlip(bool &show, bool &textureNeedsUpdate) {
        if(show){
            VerticalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Vertical Flip Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }

    // #TODO: do this shit later
    void applyMerge(bool &show, bool &textureNeedsUpdate) {
        if(show){
            Image merge_image("assets/SampleImages/luffy.jpg");
            MergeFilter filter(merge_image); 
            processor.applyFilter(filter);
            std::cout << "Applied Merge Filter\n";
            
            show = false;
        }
    }
    void applyRotate(bool &show, bool &textureNeedsUpdate) {
    static int currentItem = 0;
    const char* items[] = {"90", "180", "270"};
    int values[] = {90, 180, 270};

    static Image originalImage;
    static bool init = false;
    if(show){
        ImGui::Begin("Rotate Parameters", &show);

        if(!init){
            originalImage = processor.getCurrentImage();
            init = true;
        }

        ImGui::Text("Angle:");
        ImGui::SameLine();
        bool changed = false;
        if(ImGui::Combo("Angle", &currentItem, items, IM_ARRAYSIZE(items))) changed = true; // fixed

        if(changed){
            processor.setImage(originalImage);
            RotateFilter filter(values[currentItem]); 
            processor.applyFilter(filter);
            textureNeedsUpdate = true;
        }
        ImGui::Separator();

        if(ImGui::Button("Apply")){
            RotateFilter filter(values[currentItem]); 
            processor.setImage(originalImage);
            processor.applyFilter(filter);
            std::cout << "Rotated by: " << values[currentItem] << std::endl;
            show = false;
            init = false;
            textureNeedsUpdate = true;
        }

        ImGui::SameLine();
        if(ImGui::Button("Cancel")){
            processor.setImage(originalImage);
            show = false;
            init = false;
            textureNeedsUpdate = true;
        }

        ImGui::End();
    }else init = false;
}

    void applyOutline(bool &show, bool &textureNeedsUpdate) {
        if(show){
            OutlineFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Outline Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }
    void applyPurple(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Purple Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, 0.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                PurpleFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                PurpleFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Purple filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }

    void applyInfrared(bool &show, bool &textureNeedsUpdate) {
        if(show){
            InfraredFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Infrared Filter\n";
            
            show = false;
            textureNeedsUpdate = true;
        }
    }

    void applyWave(bool &show, bool &textureNeedsUpdate) {
        static float amplitude, wavelength;
        static Image originalImage;
        static bool init = false;
        if(show){
            ImGui::Begin("Wave Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            bool changed = false;
            
            ImGui::Text("Amplitude:");
            ImGui::SameLine();
            if(ImGui::InputFloat("##Amplitude", &amplitude, 0, 0))changed = true;
            
            ImGui::Text("Wavelength:");
            ImGui::SameLine();
            if(ImGui::InputFloat("##Wavelength", &wavelength, 0, 0))changed = true;

            if(changed){
                processor.setImage(originalImage);
                WaveFilter filter(amplitude, wavelength);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            
            ImGui::Separator();
            
            if(ImGui::Button("Apply")){
                WaveFilter filter(amplitude, wavelength);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Wave filter applied with Parameters: " << amplitude << " " << wavelength << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }

    // #TODO: change oil painting to a combo box
    void applyOilPainting(bool &show, bool &textureNeedsUpdate) {
        
        static int currentItem = 0;
        const char* items[] = {"Low", "Medium", "High"};
        int values[] = {10, 20, 40};

        static Image originalImage;
        static bool init = false;
        if(show){
            ImGui::Begin("Oil Painting Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }

            ImGui::Text("Intenisty:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::Combo("Intenisty", &currentItem, items, IM_ARRAYSIZE(items))) changed = true;

            if(changed){
                processor.setImage(originalImage);
                OilPaintingFilter filter(5, values[currentItem]); 
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
            }
            ImGui::Separator();

            if(ImGui::Button("Apply")){
                OilPaintingFilter filter(5, values[currentItem]); 
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if(ImGui::Button("Cancel")){
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }

    void applyContrast(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Contrast Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                ContrastFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                ContrastFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Contrast filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }

    void applyRetro(bool &show, bool &textureNeedsUpdate) {
        if(show){
            RetroFilter filter;
            processor.applyFilter(filter);
            std::cout << "Applied Retro Filter\n";
            show = false;
            textureNeedsUpdate = true;
        }
    }

    void applySaturation(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Saturation Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                SaturationFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                SaturationFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Saturation filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }
    void applySkew(bool &show, bool &textureNeedsUpdate) {
        static float Angle = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Skew Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Angle:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##AngleSlider", &Angle, -90.0f, 90.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##AngleInput", &Angle, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                SkewFilter filter(Angle);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                SkewFilter filter(Angle);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Skew filter applied with Angle: " << Angle << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }
    void applyVignette(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Vignette Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                VigentteFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                VigentteFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Vignette filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }
    void applyWarmth(bool &show, bool &textureNeedsUpdate) {
        static float factor = 0.0f;
        static Image originalImage;
        static bool init = false;
        if (show) {
            ImGui::Begin("Warmth Parameters", &show);

            if(!init){
                originalImage = processor.getCurrentImage();
                init = true;
            }


            ImGui::Text("Factor:");
            ImGui::SameLine();
            bool changed = false;
            if(ImGui::SliderFloat("##FactorSlider", &factor, -3.0f, 3.0f, "%.2f"))changed = true;

            
            ImGui::SameLine();
            if(ImGui::InputFloat("##FactorInput", &factor, 0.0f, 0.0f, "%.2f"))changed = true;

            if(changed){
                processor.setImage(originalImage);
                WarmthFilter filter(factor);
                processor.applyFilter(filter);
                textureNeedsUpdate = true;
                
            }
            ImGui::Separator();

            if (ImGui::Button("Apply")) {
                WarmthFilter filter(factor);
                processor.setImage(originalImage);
                processor.applyFilter(filter);
                std::cout << "Warmth filter applied with factor: " << factor << std::endl;
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                processor.setImage(originalImage);
                show = false;
                init = false;
                textureNeedsUpdate = true;
            }

            ImGui::End();
        }else init = false;
    }

private:
    ImageProcessor &processor;    
};