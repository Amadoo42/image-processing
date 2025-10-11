#pragma once
#include "core/ImageProcessor.h"
void filtersMenu(ImageProcessor &processor) {
        if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Grayscale")) {
            GrayscaleFilter filter;
            processor.applyFilter(filter);
            std::cout << "Grayscale filter applied." << std::endl;
        }

        if (ImGui::Button("Invert")) {
            InvertFilter filter;
            processor.applyFilter(filter);
            std::cout << "Invert filter applied." << std::endl;
        }

        if (ImGui::Button("Blur")) {
            BlurFilter filter;
            processor.applyFilter(filter);
            std::cout << "Blur filter applied." << std::endl;
        }

        if (ImGui::Button("Wave")) {
            WaveFilter filter;
            processor.applyFilter(filter);
            std::cout << "Wave filter applied." << std::endl;
        }

        if (ImGui::Button("Black & white")) {
            BlackAndWhiteFilter filter;
            processor.applyFilter(filter);
            std::cout << "Black & white filter applied." << std::endl;
        }
        
        //TODO : add parameters
        if (ImGui::Button("Crop")) {
            CropFilter filter(0,0,500,500);
            processor.applyFilter(filter);
            std::cout << "Crop filter applied." << std::endl;
        }
        
        if (ImGui::Button("Resize")) {
            ResizeFilter filter;
            processor.applyFilter(filter);
            std::cout << "Resize filter applied." << std::endl;
        }

        //TODO: ADD SLIDERS
        if (ImGui::Button("Darken")) {
            DarkenFilter filter;
            processor.applyFilter(filter);
            std::cout << "Darken filter applied." << std::endl;
        }
        
        //TODO: ADD Sliders
        if (ImGui::Button("Lighten")) {
            LightenFilter filter;
            processor.applyFilter(filter);
            std::cout << "Lighten filter applied." << std::endl;
        }

        //TODO: add parameter here
        if (ImGui::Button("Frame")) {
            FrameFilter filter("../assets/SampleImages/luffy.jpg");
            processor.applyFilter(filter);
            std::cout << "Frame filter applied." << std::endl;
        }

        if (ImGui::Button("Horizontal Flip")) {
            HorizontalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Horizontal Flip filter applied." << std::endl;
        }

        if (ImGui::Button("Vertical Flip")) {
            VerticalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Vertical Flip filter applied." << std::endl;
        }

        if (ImGui::Button("Merge")) {
            MergeFilter filter("../assets/SampleImages/luffy.jpg");
            processor.applyFilter(filter);
            std::cout << "Merge filter applied." << std::endl;
        }

        //TODO: add parameter
        if (ImGui::Button("Rotate")) {
            RotateFilter filter(90);
            processor.applyFilter(filter);
            std::cout << "Rotate filter applied." << std::endl;
        }

        if (ImGui::Button("Invert")) {
            InvertFilter filter(90);
            processor.applyFilter(filter);
            std::cout << "Invert filter applied." << std::endl;
        }
        
        if (ImGui::Button("Outline")) {
            OutlineFilter filter;
            processor.applyFilter(filter);
            std::cout << "Outline filter applied." << std::endl;
        }

        if (ImGui::Button("Rotate")) {
            RotateFilter filter(90);
            processor.applyFilter(filter);
            std::cout << "Rotate filter applied." << std::endl;
        }

        if (ImGui::Button("Purple")) {
            PurpleFilter filter;
            processor.applyFilter(filter);
            std::cout << "Purple filter applied." << std::endl;
        }

        if (ImGui::Button("Infrared")) {
            InfraredFilter filter;
            processor.applyFilter(filter);
            std::cout << "Infrared filter applied." << std::endl;
        }

        //TODO: add slider
        if (ImGui::Button("Wave")) {
            WaveFilter filter;
            processor.applyFilter(filter);
            std::cout << "Wave filter applied." << std::endl;
        }

        if (ImGui::Button("Oil Painting")) {
            OilPaintingFilter filter;
            processor.applyFilter(filter);
            std::cout << "Oil painting filter applied." << std::endl;
        }

        if (ImGui::Button("Retro")) {
            RetroFilter filter;
            processor.applyFilter(filter);
            std::cout << "Retro filter applied." << std::endl;
        }

        //TODO: Add parameters
        if (ImGui::Button("Skew")) {
            SkewFilter filter(45);
            processor.applyFilter(filter);
            std::cout << "Skew filter applied." << std::endl;
        }
        
        //TODO: Add sliders to the next 4
        if (ImGui::Button("Warmth")) {
            WarmthFilter filter;
            processor.applyFilter(filter);
            std::cout << "Warmth filter applied." << std::endl;
        }

        if (ImGui::Button("Saturation")) {
            SaturationFilter filter;
            processor.applyFilter(filter);
            std::cout << "Saturation filter applied." << std::endl;
        }

        if (ImGui::Button("Contrast")) {
            ContrastFilter filter;
            processor.applyFilter(filter);
            std::cout << "Contrast filter applied." << std::endl;
        }

        if (ImGui::Button("Vigentte")) {
            VigentteFilter filter;
            processor.applyFilter(filter);
            std::cout << "Vigentte filter applied." << std::endl;
        }

    }

}