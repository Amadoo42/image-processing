#pragma once
#include <memory>
#include <string>
#include <vector>

static std::string runCapture1(const char* cmd) {
    std::array<char, 4096> buf;
    std::string result;
    std::unique_ptr<FILE, int (*)(FILE*)> pipe(popen(cmd, "r"), (int (*)(FILE*))pclose);
    if (!pipe) return {};
    while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) result += buf.data();
    if (!result.empty() && result.back() == '\n') result.pop_back();
    return result;
}

std::string openFileDialog_Linux() {
    // try kdialog first
    std::string out = runCapture1("which kdialog >/dev/null 2>&1 && kdialog --getopenfilename 2>/dev/null || true");
    if(!out.empty()) return out;
    // now try zenity cuz kdialog failed for some reason
    out = runCapture1("which zenity >/dev/null 2>&1 && "
        "zenity --file-selection --title=\"Select an image\" --file-filter='Images | *.png *.jpg *.jpeg *.bmp *.gif' 2>/dev/null || true");
    if(!out.empty()) return out;
    return "";
}

std::string saveFileDialog_Linux() {
    // try kdialog first
    std::string out = runCapture1("which kdialog >/dev/null 2>&1 && kdialog --getsavefilename 2>/dev/null || true");
    if(!out.empty()) return out;
    // now try zenity cuz kdialog failed for some reason
    out = runCapture1("which zenity >/dev/null 2>&1 && "
        "zenity --file-selection --save --title=\"Save an image\" --file-filter='Images | *.png *.jpg *.jpeg *.bmp *.gif' 2>/dev/null || true");
    if(!out.empty()) return out;
    return "";
}

// Returns a newline-separated list of selected file paths.
// Caller should split on '\n' and ignore empty lines.
inline std::string openMultipleFilesDialog_Linux() {
    // Try kdialog first
    std::string out = runCapture1("which zenity >/dev/null 2>&1 && "
                                  "zenity --file-selection --title=\"Select images\" --file-filter='Images | *.png *.jpg *.jpeg *.bmp *.gif' --multiple --separator='\n' 2>/dev/null || "
                                  "(which kdialog >/dev/null 2>&1 && kdialog --getopenfilename --multiple --separate-output 2>/dev/null) || true");
    if(!out.empty()) return out;
    // last resort
    if(!out.empty()) return out;
    return "";
}

#ifdef _WIN32
#include <windows.h>

inline std::string openFileDialog_Windows(bool save = false, bool multiple = false) {
    OPENFILENAMEA ofn;
    CHAR fileBuffer[8192] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = fileBuffer;
    ofn.nMaxFile = sizeof(fileBuffer);
    ofn.lpstrFilter = "Images\0*.png;*.jpg;*.jpeg;*.bmp;*.gif\0All\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | (multiple ? OFN_ALLOWMULTISELECT | OFN_EXPLORER : 0);
    BOOL ok = FALSE;
    if (save) ok = GetSaveFileNameA(&ofn);
    else ok = GetOpenFileNameA(&ofn);
    if (!ok) return "";
    // If multiple, buffer contains directory followed by files
    if (multiple) {
        std::string dir = ofn.lpstrFile;
        size_t len = dir.size();
        std::vector<std::string> files;
        const char* p = ofn.lpstrFile + len + 1;
        if (*p == '\0') return dir; // single selection
        while (*p) {
            files.emplace_back(p);
            p += files.back().size() + 1;
        }
        std::string joined;
        for (size_t i = 0; i < files.size(); ++i) {
            if (i) joined += '\n';
            joined += dir + "\\" + files[i];
        }
        return joined;
    }
    return std::string(ofn.lpstrFile);
}
#endif