#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <optional>
#include <cctype>
#include <iostream>

#include "FilterDefs.h"
#include "../core/ImageProcessor.h"

// Include all filters so we can instantiate them when applying presets
#include "../filters/GrayscaleFilter.h"
#include "../filters/InvertFilter.h"
#include "../filters/BlackAndWhiteFilter.h"
#include "../filters/BlurFilter.h"
#include "../filters/ContrastFilter.h"
#include "../filters/CropFilter.h"
#include "../filters/DarkenFilter.h" // used for Brightness
#include "../filters/FrameFilter.h"
#include "../filters/HorizontalFlipFilter.h"
#include "../filters/InfraredFilter.h"
#include "../filters/WaveFilter.h"
#include "../filters/MergeFilter.h"
#include "../filters/OilPaintingFilter.h"
#include "../filters/OutlineFilter.h"
#include "../filters/PurpleFilter.h"
#include "../filters/ResizeFilter.h"
#include "../filters/RetroFilter.h"
#include "../filters/RotateFilter.h"
#include "../filters/SaturationFilter.h"
#include "../filters/SkewFilter.h"
#include "../filters/VigentteFilter.h"
#include "../filters/VerticalFlipFilter.h"
#include "../filters/WarmthFilter.h"

struct FilterStep {
    FilterType type;
    std::vector<double> params; // numeric parameters for the filter, if any
    std::string path;            // optional extra path (e.g., for Frame/Merge)
};

struct PresetDefinition {
    std::string name;
    std::vector<FilterStep> steps;
};

inline FilterType filterTypeFromName(const std::string &name) {
    if (name == "None") return FilterType::None;
    if (name == "Grayscale") return FilterType::Grayscale;
    if (name == "Invert") return FilterType::Invert;
    if (name == "Blur") return FilterType::Blur;
    if (name == "Black & White") return FilterType::BlackAndWhite;
    if (name == "Crop") return FilterType::Crop;
    if (name == "Resize") return FilterType::Resize;
    if (name == "Brightness") return FilterType::Brightness;
    if (name == "Frame") return FilterType::Frame;
    if (name == "Horizontal Flip") return FilterType::HorizontalFlip;
    if (name == "Vertical Flip") return FilterType::VerticalFlip;
    if (name == "Merge") return FilterType::Merge;
    if (name == "Rotate") return FilterType::Rotate;
    if (name == "Outline") return FilterType::Outline;
    if (name == "Purple") return FilterType::Purple;
    if (name == "Infrared") return FilterType::Infrared;
    if (name == "Wave") return FilterType::Wave;
    if (name == "Oil Painting") return FilterType::OilPainting;
    if (name == "Retro") return FilterType::Retro;
    if (name == "Saturation") return FilterType::Saturation;
    if (name == "Contrast") return FilterType::Contrast;
    if (name == "Skew") return FilterType::Skew;
    if (name == "Vignette") return FilterType::Vignette;
    if (name == "Warmth") return FilterType::Warmth;
    return FilterType::None;
}

class PresetManager {
public:
    PresetManager() : storagePath("presets.dat") {}

    void clearSession() { sessionSteps.clear(); }
    bool isSessionEmpty() const { return sessionSteps.empty(); }
    const std::vector<FilterStep>& getSessionSteps() const { return sessionSteps; }
    void recordStep(const FilterStep &step) { sessionSteps.push_back(step); }

    const std::vector<PresetDefinition>& getPresets() const { return presets; }

    std::vector<std::string> getPresetNames() const {
        std::vector<std::string> names;
        names.reserve(presets.size());
        for (const auto &p : presets) names.push_back(p.name);
        return names;
    }

    int findPresetIndexByName(const std::string &name) const {
        for (size_t i = 0; i < presets.size(); ++i) if (presets[i].name == name) return (int)i;
        return -1;
    }

    void addPreset(const std::string &name, const std::vector<FilterStep> &steps) {
        PresetDefinition p{ name, steps };
        int idx = findPresetIndexByName(name);
        if (idx >= 0) presets[idx] = p; else presets.push_back(p);
        saveToDisk();
    }

    void deletePreset(int index) {
        if (index < 0 || index >= (int)presets.size()) return;
        presets.erase(presets.begin() + index);
        saveToDisk();
    }

    void renamePreset(int index, const std::string &newName) {
        if (index < 0 || index >= (int)presets.size()) return;
        presets[index].name = newName;
        saveToDisk();
    }

    bool applyPreset(ImageProcessor &processor, const PresetDefinition &preset) const {
        bool ok = true;
        for (const auto &step : preset.steps) {
            if (!applyStep(processor, step)) ok = false;
        }
        return ok;
    }

    void loadFromDisk() {
        presets.clear();
        std::ifstream in(storagePath);
        if (!in.good()) return;
        std::string line;
        PresetDefinition current;
        bool inPreset = false;
        while (std::getline(in, line)) {
            if (line.rfind("PRESET ", 0) == 0) {
                if (inPreset) presets.push_back(current);
                current = PresetDefinition{};
                current.name = line.substr(7);
                trim(current.name);
                inPreset = true;
            } else if (line.rfind("STEP ", 0) == 0 && inPreset) {
                FilterStep step{};
                std::string typeName = line.substr(5);
                trim(typeName);
                step.type = filterTypeFromName(typeName);
                std::string l2;
                if (std::getline(in, l2)) {
                    if (l2.rfind("PARAMS ", 0) == 0) {
                        std::istringstream iss(l2.substr(7));
                        int n = 0; iss >> n;
                        step.params.clear();
                        for (int i = 0; i < n; ++i) { double v = 0.0; iss >> v; step.params.push_back(v); }
                    } else {
                        in.seekg(-(std::streamoff)l2.size() - 1, std::ios_base::cur);
                    }
                }
                std::string l3;
                if (std::getline(in, l3)) {
                    if (l3.rfind("PATH ", 0) == 0) {
                        step.path = l3.size() > 5 ? l3.substr(5) : std::string();
                    } else {
                        in.seekg(-(std::streamoff)l3.size() - 1, std::ios_base::cur);
                    }
                }
                current.steps.push_back(step);
            } else if (line == "END" && inPreset) {
                presets.push_back(current);
                current = PresetDefinition{};
                inPreset = false;
            }
        }
        if (inPreset) presets.push_back(current);
    }

    void saveToDisk() const {
        std::ofstream out(storagePath, std::ios::trunc);
        if (!out.good()) return;
        for (const auto &p : presets) {
            out << "PRESET " << p.name << "\n";
            for (const auto &s : p.steps) {
                out << "STEP " << filterTypeName(s.type) << "\n";
                out << "PARAMS " << s.params.size();
                for (double v : s.params) out << ' ' << v;
                out << "\n";
                out << "PATH " << s.path << "\n";
            }
            out << "END\n";
        }
    }

    static void ensureOutputFolder(const std::string &folder) {
        std::error_code ec;
        std::filesystem::create_directories(folder, ec);
    }

private:
    std::string storagePath;
    std::vector<PresetDefinition> presets;
    std::vector<FilterStep> sessionSteps;

    static void trim(std::string &s) {
        size_t a = 0, b = s.size();
        while (a < b && std::isspace((unsigned char)s[a])) ++a;
        while (b > a && std::isspace((unsigned char)s[b - 1])) --b;
        s = s.substr(a, b - a);
    }

    static double paramOr(const FilterStep &s, size_t idx, double def) {
        return idx < s.params.size() ? s.params[idx] : def;
    }

    bool applyStep(ImageProcessor &processor, const FilterStep &step) const {
        switch (step.type) {
            case FilterType::Grayscale: { GrayscaleFilter f; processor.applyFilter(f); return true; }
            case FilterType::Invert: { InvertFilter f; processor.applyFilter(f); return true; }
            case FilterType::BlackAndWhite: { BlackAndWhiteFilter f; processor.applyFilter(f); return true; }
            case FilterType::HorizontalFlip: { HorizontalFlipFilter f; processor.applyFilter(f); return true; }
            case FilterType::VerticalFlip: { VerticalFlipFilter f; processor.applyFilter(f); return true; }
            case FilterType::Retro: { RetroFilter f; processor.applyFilter(f); return true; }
            case FilterType::Outline: { OutlineFilter f; processor.applyFilter(f); return true; }
            case FilterType::Infrared: { InfraredFilter f; processor.applyFilter(f); return true; }
            case FilterType::Brightness: { double factor = paramOr(step, 0, 1.0); DarkenFilter f((float)factor); processor.applyFilter(f); return true; }
            case FilterType::Contrast: { double factor = paramOr(step, 0, 1.0); ContrastFilter f(factor); processor.applyFilter(f); return true; }
            case FilterType::Saturation: { double factor = paramOr(step, 0, 1.0); SaturationFilter f(factor); processor.applyFilter(f); return true; }
            case FilterType::Warmth: { double factor = paramOr(step, 0, 1.0); WarmthFilter f(factor); processor.applyFilter(f); return true; }
            case FilterType::Vignette: { double factor = paramOr(step, 0, 1.0); VigentteFilter f(factor); processor.applyFilter(f); return true; }
            case FilterType::Blur: { int kSize = (int)paramOr(step, 0, 13.0); double sigma = paramOr(step, 1, 2.0); if (kSize % 2 == 0) kSize += 1; BlurFilter f(kSize, sigma); processor.applyFilter(f); return true; }
            case FilterType::Rotate: { double deg = paramOr(step, 0, 0.0); RotateFilter f(deg, true); processor.applyFilter(f); return true; }
            case FilterType::Skew: { double deg = paramOr(step, 0, 0.0); SkewFilter f(deg); processor.applyFilter(f); return true; }
            case FilterType::Wave: { double a = paramOr(step, 0, 0.0); double w = paramOr(step, 1, 1.0); WaveFilter f((float)a, (float)w); processor.applyFilter(f); return true; }
            case FilterType::OilPainting: { int radius = (int)paramOr(step, 0, 5.0); int intensity = (int)paramOr(step, 1, 20.0); OilPaintingFilter f(radius, intensity); processor.applyFilter(f); return true; }
            case FilterType::Resize: { int w = (int)paramOr(step, 0, 0.0); int h = (int)paramOr(step, 1, 0.0); if (w <= 0 || h <= 0) return false; ResizeFilter f(w, h); processor.applyFilter(f); return true; }
            case FilterType::Crop: { int x = (int)paramOr(step, 0, 0.0); int y = (int)paramOr(step, 1, 0.0); int w = (int)paramOr(step, 2, 0.0); int h = (int)paramOr(step, 3, 0.0); if (w <= 0 || h <= 0) return false; CropFilter f(x, y, w, h); processor.applyFilter(f); return true; }
            case FilterType::Frame: { if (step.path.empty()) return false; Image frame(step.path); FrameFilter f(frame); processor.applyFilter(f); return true; }
            case FilterType::Merge: { if (step.path.empty()) return false; Image other(step.path); MergeFilter f(other); processor.applyFilter(f); return true; }
            case FilterType::None: default: return true;
        }
    }
};

inline PresetManager gPresetManager;
