#pragma once
#include "core/ImageProcessor.h"
#include "FilterParameters.h"

void filtersMenu(ImageProcessor &processor) {
    FilterParameters para(processor);

    if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {

        int count = 0; // counter for layout

        auto addButton = [&](const char* label, auto&& func) {
            if (ImGui::Button(label, ImVec2(150, 0))) { // optional: fixed width
                func();
            }
            count++;
            if (count % 4 != 0) ImGui::SameLine(); // 4 per row
        };

        addButton("Grayscale", [&]{
            GrayscaleFilter filter;
            processor.applyFilter(filter);
            std::cout << "Grayscale filter applied." << std::endl;
        });

        addButton("Invert", [&]{
            InvertFilter filter;
            processor.applyFilter(filter);
            std::cout << "Invert filter applied." << std::endl;
        });

        addButton("Blur", [&]{
            BlurFilter filter;
            processor.applyFilter(filter);
            std::cout << "Blur filter applied." << std::endl;
        });

        addButton("Black & white", [&]{
            BlackAndWhiteFilter filter;
            processor.applyFilter(filter);
            std::cout << "Black & white filter applied." << std::endl;
        });

        addButton("Crop", [&]{
            CropFilter filter(0, 0, 500, 500);
            processor.applyFilter(filter);
            std::cout << "Crop filter applied." << std::endl;
        });

        addButton("Resize", [&]{
            ResizeFilter filter(500, 500);
            processor.applyFilter(filter);
            std::cout << "Resize filter applied." << std::endl;
        });

        addButton("Darken", [&]{
            DarkenFilter filter;
            processor.applyFilter(filter);
            std::cout << "Darken filter applied." << std::endl;
        });

        addButton("Lighten", [&]{
            LightenFilter filter;
            processor.applyFilter(filter);
            std::cout << "Lighten filter applied." << std::endl;
        });

        addButton("Frame", [&]{
            Image frame_image("../assets/frames/fancy.png");
            FrameFilter filter(frame_image);
            processor.applyFilter(filter);
            std::cout << "Frame filter applied." << std::endl;
        });

        addButton("Horizontal Flip", [&]{
            HorizontalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Horizontal Flip filter applied." << std::endl;
        });

        addButton("Vertical Flip", [&]{
            VerticalFlipFilter filter;
            processor.applyFilter(filter);
            std::cout << "Vertical Flip filter applied." << std::endl;
        });

        addButton("Merge", [&]{
            Image merge_image("../assets/SampleImages/luffy.jpg");
            MergeFilter filter(merge_image);
            processor.applyFilter(filter);
            std::cout << "Merge filter applied." << std::endl;
        });

        addButton("Rotate", [&]{
            RotateFilter filter(90);
            processor.applyFilter(filter);
            std::cout << "Rotate filter applied." << std::endl;
        });

        addButton("Outline", [&]{
            OutlineFilter filter;
            processor.applyFilter(filter);
            std::cout << "Outline filter applied." << std::endl;
        });

        addButton("Purple", [&]{
            PurpleFilter filter;
            processor.applyFilter(filter);
            std::cout << "Purple filter applied." << std::endl;
        });

        addButton("Infrared", [&]{
            InfraredFilter filter;
            processor.applyFilter(filter);
            std::cout << "Infrared filter applied." << std::endl;
        });

        addButton("Wave", [&]{
            WaveFilter filter;
            processor.applyFilter(filter);
            std::cout << "Wave filter applied." << std::endl;
        });

        addButton("Oil Painting", [&]{
            OilPaintingFilter filter;
            processor.applyFilter(filter);
            std::cout << "Oil painting filter applied." << std::endl;
        });

        addButton("Retro", [&]{
            RetroFilter filter;
            processor.applyFilter(filter);
            std::cout << "Retro filter applied." << std::endl;
        });

        addButton("Skew", [&]{
            SkewFilter filter(45);
            processor.applyFilter(filter);
            std::cout << "Skew filter applied." << std::endl;
        });

        // Define persistent variables outside the button callback
        static bool showWarmthWindow = false;

        // Your Warmth button
        addButton("Warmth", [&]{
            showWarmthWindow = true; // open the parameter window when clicked
        });
        para.applyWarmth(showWarmthWindow);
        // Draw the warmth parameter window every frame (only when opened)
        


        addButton("Saturation", [&]{
            SaturationFilter filter;
            processor.applyFilter(filter);
            std::cout << "Saturation filter applied." << std::endl;
        });

        addButton("Contrast", [&]{
            ContrastFilter filter;
            processor.applyFilter(filter);
            std::cout << "Contrast filter applied." << std::endl;
        });

        addButton("Vignetting", [&]{
            VigentteFilter filter;
            processor.applyFilter(filter);
            std::cout << "Vignetting filter applied." << std::endl;
        });
    }
}
