#pragma once

// Minimal icon helpers for Font Awesome compatible fonts

#ifndef ICON_FA_FOLDER_OPEN
#define ICON_FA_FOLDER_OPEN u8"\uf07c"
#endif
#ifndef ICON_FA_FLOPPY_DISK
#define ICON_FA_FLOPPY_DISK u8"\uf0c7" // Save
#endif
#ifndef ICON_FA_FLOPPY_DISK_CIRCLE_ARROW_RIGHT
#define ICON_FA_FLOPPY_DISK_CIRCLE_ARROW_RIGHT u8"\ue180" // Save As (approx.)
#endif
#ifndef ICON_FA_RIGHT_FROM_BRACKET
#define ICON_FA_RIGHT_FROM_BRACKET u8"\uf2f5" // Exit
#endif
#ifndef ICON_FA_ROTATE_LEFT
#define ICON_FA_ROTATE_LEFT u8"\uf2ea" // Undo
#endif
#ifndef ICON_FA_ROTATE_RIGHT
#define ICON_FA_ROTATE_RIGHT u8"\uf2f9" // Redo
#endif
#ifndef ICON_FA_ARROWS_ROTATE
#define ICON_FA_ARROWS_ROTATE u8"\uf021" // Reset/Refresh
#endif
#ifndef ICON_FA_GEAR
#define ICON_FA_GEAR u8"\uf013" // Preferences
#endif
#ifndef ICON_FA_QUESTION_CIRCLE
#define ICON_FA_QUESTION_CIRCLE u8"\uf059" // About
#endif
#ifndef ICON_FA_BOOK
#define ICON_FA_BOOK u8"\uf02d" // Docs
#endif
#ifndef ICON_FA_SQUARE_SPLIT
#define ICON_FA_SQUARE_SPLIT u8"\ue43c" // Compare (approx.)
#endif

#include <string>
#include <cstring>

inline std::string iconLabel(const char* icon, const char* label)
{
    std::string s;
    s.reserve(8 + std::strlen(label));
    s.append(icon);
    s.push_back(' ');
    s.append(label);
    return s;
}
