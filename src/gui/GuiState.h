/*
  Declares all globally accessible variables that manage the UI's runtime state,
  such as zoom_level, pan_offset, gSelectedFilter, textureNeedsUpdate,
  and the current selection mode (gSelectionTool).
*/

#pragma once

#include "imgui.h"
#include <GL/gl.h>
#include <string>
#include "FilterDefs.h"
#include "SelectionTools.h"

class Image;

extern bool is_dark_theme;
extern int preferences_theme;
extern float zoom_level;
extern ImVec2 pan_offset;
extern bool compareMode;
extern bool showPreferencesWindow;
extern bool showAboutWindow;
extern FilterType gSelectedFilter;
extern GLuint currentTextureID;
extern bool textureNeedsUpdate;
extern std::string statusBarMessage;

extern SelectionToolMode gSelectionTool;
extern bool gSelectionInverseApply;
extern int  gMagicWandTolerance;
extern bool gHasActiveRectDrag;
extern ImVec2 gRectDragStart;
extern ImVec2 gRectDragEnd;
extern Image gOriginalImageForPreview;
extern bool gHasOriginalImageForPreview;
extern int gUndoHistorySize;
extern char gBatchOutputDirectory[256];

extern char gSearchBuffer[128];
extern ImVec2 gLastCanvasAvail;
extern float kLeftPanelPct;
extern float kRightPanelPct;
extern bool  gPreviewCacheNeedsUpdate;
extern std::string gCurrentImagePath;
extern int gImageSessionId;
extern bool showPresetsWindow;
extern bool showBatchWindow;
extern bool showSaveCurrentPresetPopup;
extern bool showPresetBuilderWindow;

void guiSetCurrentImagePath(const std::string &path);
