#include <memory>
#include <string>

static std::string runCapture1(const char* cmd) {
    std::array<char, 4096> buf;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
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