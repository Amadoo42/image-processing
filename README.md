# SR-FX Image Processing Application (GUI)

A modern, C++17 image processing application with a real-time graphical interface. SR-FX lets you load images, apply 20+ filters and adjustments, preview changes live, and save the results. It includes selection tools, presets, batch processing, and an undo/redo history.


### 1. Project Overview

SR-FX is a lightweight image editor built on Dear ImGui, SDL2, and OpenGL. It focuses on fast, interactive editing with live previews and simple, discoverable controls. Choose a filter from the left panel, adjust parameters using sliders and inputs, see the result instantly in the canvas, and export when satisfied.

- Real-time preview for most parameterized filters
- Selection tools (rectangle, magic wand) for targeted edits
- Preset builder and batch processing
- Undo/Redo with configurable history


### 2. Tools and Technologies Used

- C++17
- CMake (>= 3.10)
- SDL2 (windowing, input)
- OpenGL 3.0+ (rendering), GLSL 130
- Dear ImGui (immediate-mode GUI)
- stb_image / stb_image_write (image I/O)


### 3. Setup and Execution

#### Prerequisites
- A C++17 compiler (g++ or clang++)
- CMake 3.10+
- SDL2 development libraries
- OpenGL development libraries and headers
- Make or Ninja build tools

For Ubuntu/Debian-based Linux:
```bash
sudo apt update
sudo apt install -y build-essential cmake libsdl2-dev libgl1-mesa-dev xorg-dev
```
For macOS (Homebrew):
```bash
brew install cmake sdl2
# OpenGL headers are provided by macOS; Xcode Command Line Tools recommended
```
For Windows:
- Install Visual Studio with C++ tooling
- Install SDL2 development package (e.g., via vcpkg or prebuilt binaries)
- Let CMake generate a VS solution; ensure SDL2 include/libs are discoverable

#### Installation
```bash
# Clone
git clone <REPO_URL>
cd <REPO_FOLDER>

# Configure & build (Release recommended)
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j
```

Note: The build system copies the `assets/` folder into the build directory so fonts and frames are available at runtime (see `CMakeLists.txt`).

#### Running the Program
From the build directory:
```bash
./image_processor
```
Keyboard shortcuts:
- Ctrl+O: Open image
- Ctrl+S: Save image
- Ctrl+Z: Undo
- Ctrl+Y: Redo

Supported formats (runtime):
- Input: JPG, PNG, BMP, TGA
- Output: JPG, PNG, BMP


### 4. Directory Structure

```
.
├── CMakeLists.txt
├── src/
│   ├── main.cpp                         # App entry (SDL2 + ImGui + OpenGL setup)
│   ├── core/
│   │   └── ImageProcessor.h             # Image state, history, selection, apply filters
│   ├── filters/
│   │   ├── Filter.h                     # Base filter interface
│   │   ├── WaveFilter.h                 # Wave effect (sine-based distortion)
│   │   ├── ContrastFilter.h             # Contrast adjustment
│   │   ├── SaturationFilter.h           # Saturation adjustment
│   │   ├── VigentteFilter.h             # Vignette effect
│   │   ├── BlackAndWhiteFilter.h        # B&W thresholding
│   │   ├── BlurFilter.h                 # Gaussian blur
│   │   ├── CropFilter.h                 # Crop
│   │   ├── DarkenFilter.h               # Brightness control backend
│   │   ├── FlipFilter.h                 # Horizontal/vertical flip
│   │   ├── FrameFilter.h                # Overlay frame image
│   │   ├── GrayscaleFilter.h            # Luminance grayscale
│   │   ├── InfraredFilter.h             # Infrared stylization
│   │   ├── InvertFilter.h               # Color invert
│   │   ├── MergeFilter.h                # Blend two images
│   │   ├── OilPaintingFilter.h          # Oil-painting stylization
│   │   ├── OutlineFilter.h              # Edge outline
│   │   ├── PurpleFilter.h               # Purple tint
│   │   ├── ResizeFilter.h               # Resize
│   │   ├── RetroFilter.h                # Retro/TV effect
│   │   ├── RotateFilter.h               # Rotation
│   │   ├── SkewFilter.h                 # Horizontal skew
│   │   ├── WarmthFilter.h               # Color temperature
│   │   └── ...                          # Other helpers
│   └── gui/
│       ├── RenderGUI.h                  # Main window layout & panels
│       ├── FiltersMenu.h                # Left filter catalog with previews
│       ├── FilterParameters.h           # Parameter UIs with live preview
│       ├── FilterPreview.h              # Small preview grid
│       ├── FilterDefs.h                 # Filter enum & names (single source of truth)
│       ├── ui/
│       │   ├── TopNavBar.h              # File, presets, batch, help
│       │   ├── LeftParamsPanel.h        # Parameter panel (left)
│       │   ├── RightPanel.h             # Sidebar (history, selection, etc.)
│       │   ├── ImageCanvas.h            # Zoom/pan canvas & compare
│       │   └── BottomToolbar.h          # Status bar and controls
│       ├── LoadTexture.h                # Upload CPU image to OpenGL texture
│       ├── CompareView.h                # Before/after comparison
│       ├── SelectionTools.h             # Rectangle & magic wand selection
│       └── PresetManager.h              # Preset storage & batch apply
├── external/
│   ├── imgui/                           # Dear ImGui (vendored)
│   ├── stb_image.h
│   └── stb_image_write.h
├── assets/
│   ├── fonts/                           # Inter, Font Awesome, etc.
│   ├── frames/                          # Example frame overlays
│   └── SampleImages/                    # Example input images
├── LICENSE
└── README.md
```


### 5. GUI User Guide

- Load a new image
  1) Use the top navigation (File → Open…) or press Ctrl+O.
  2) Pick a JPG/PNG/BMP/TGA file. The image appears on the canvas and in the status bar.

- Select and apply a filter
  1) In the left sidebar under “Filters”, choose a category (Basic Adjustments, Transformations, Effects).
  2) Click a filter (e.g., Contrast). Its parameter UI opens in the parameter panel.
  3) Adjust controls (sliders, inputs, dropdowns). For filters with parameters, you will typically see a live preview.
  4) Click “Apply” to commit. You can Undo/Redo (Ctrl+Z / Ctrl+Y).

- Preview changes in real-time
  - For parameterized filters (e.g., Blur, Brightness, Contrast, Saturation, Wave, Vignette, Warmth, Purple, Skew, Oil Painting), changes render live while you drag sliders. The app temporarily applies the filter without writing to history until you click “Apply” (or cancel).
  - Use the Compare view (toggle in canvas) to see before/after.

- Save the final image
  1) Use File → Save… or press Ctrl+S.
  2) Choose the output filename (JPG/PNG/BMP). The result is written to disk.

Advanced:
- Selections: Use rectangle or magic wand; most filters respect active selections and can invert the affected region.
- Presets & Batch: Save a chain as a preset and batch-apply it to many images from the Presets or Batch windows.
- Zoom & Pan: Mouse wheel to zoom; right-drag to pan.


### 6. Filter Index

From `src/gui/FilterDefs.h` (user-facing names):
- Grayscale
- Invert
- Blur
- Black & White
- Brightness
- Contrast
- Saturation
- Rotate
- Skew
- Flip (Horizontal / Vertical)
- Crop
- Resize
- Outline
- Purple
- Infrared
- Wave
- Oil Painting
- Retro
- Vignette
- Warmth
- Frame (overlay)
- Merge (two images)

Notes:
- Internally, `HorizontalFlip` and `VerticalFlip` are available; the UI exposes them via a single Flip panel with checkboxes.


### 7. Filter Implementation Documentation

Below are concise algorithmic descriptions of four core filters as implemented in `src/filters/*.h` and driven by the parameter UIs in `src/gui/FilterParameters.h`.

- Wave Filter (`WaveFilter.h`)
  - Purpose: Create a horizontal sine-wave distortion.
  - Parameters: `amplitude` (pixels), `wavelength` (pixels).
  - Mapping (nearest-neighbor resampling):
    - For each output pixel at integer coordinates (x, y):
      - Compute source X: \( x_s = x + a \cdot \sin(2\pi y / w) \)
      - Source Y: \( y_s = y \)
      - Clamp \( x_s \) to [0, width-1]; clamp \( y_s \) to [0, height-1].
      - Copy RGB from (\(\lfloor x_s \rceil\), \(\lfloor y_s \rceil\)).
  - Edge handling: hard clamp at image borders.
  - UI ranges: amplitude [0, 20], wavelength [0.1, 100.0] (safe-clamped).

- Contrast Adjustment (`ContrastFilter.h`)
  - Purpose: Scale distances from mid-gray (128) per channel.
  - Parameter: `factor` (>= 0; UI uses [1.0, 3.0]).
  - Per-channel transform for 8-bit RGB:
    - \( v' = \mathrm{clamp}(128 + (v - 128)\cdot f, 0, 255) \)
  - Behavior: f = 1 leaves unchanged; f > 1 increases contrast; 0 < f < 1 (not exposed in UI) would reduce contrast.

- Saturation Adjustment (`SaturationFilter.h`)
  - Purpose: Adjust colorfulness relative to luminance.
  - Parameter: `factor` (>= 0; UI uses [0.0, 3.0]).
  - Luminance (ITU-R BT.601): \( Y = 0.299R + 0.587G + 0.114B \)
  - Per-channel transform:
    - \( c' = \mathrm{clamp}(Y + (c - Y)\cdot f, 0, 255) \) for c ∈ {R, G, B}
  - Behavior: f = 0 → grayscale; f = 1 → original; f > 1 → more saturated.

- Vignette Effect (`VigentteFilter.h`)
  - Purpose: Darken toward the image corners using radial falloff.
  - Parameter: `factor` (UI uses [0.0, 3.0], default ~1.5).
  - Compute center: \( (c_x, c_y) = (\lfloor W/2 \rfloor, \lfloor H/2 \rfloor) \)
  - Max distance: \( d_{max} = \sqrt{c_x^2 + c_y^2} \)
  - For each pixel (x, y):
    - \( d = \sqrt{(x-c_x)^2 + (y-c_y)^2} \)
    - Gain: \( g = 1 - (d/d_{max}) \cdot f \), then clamp g to [0, 1]
    - Apply: \( (R',G',B') = (R,G,B)\cdot g \)
  - Effect strength increases with `factor`; 0 disables the effect.

Implementation details:
- Live previews use no-history application; clicking Apply writes to history for undo/redo.
- Selection-aware application blends filtered and original pixels using a per-pixel selection mask (see `ImageProcessor::applyFilterSelective*`).


---

License: MIT (see `LICENSE`).
