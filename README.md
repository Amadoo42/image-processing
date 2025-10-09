# Image Processing Application

A comprehensive C++ image processing application with 22+ filters and effects, featuring undo/redo functionality and an intuitive command-line interface.

## Team Information
- **Course**: CS213 - Object Oriented Programming
- **Assignment**: A1 Part 2
- **University**: Faculty of Computers and Artificial Intelligence, Cairo University

## Features

### Core Functionality
- Load and save images (PNG, JPG, BMP, TGA formats)
- Apply multiple filters sequentially
- Undo/Redo support (up to 10 steps)
- Interactive command-line menu

### Available Filters

#### Basic Filters
1. **Black & White** - Converts image to pure black and white using adaptive thresholding
2. **Grayscale** - Converts to grayscale using luminance formula
3. **Invert** - Inverts all color channels
4. **Darken** - Reduces brightness by 50%
5. **Lighten** - Increases brightness by 50%

#### Artistic Filters
6. **Purple** - Adds purple tint to the image
7. **Infrared** - Simulates infrared photography effect
8. **Oil Painting** - Creates oil painting artistic effect
9. **Retro TV** - Adds vintage TV scanline effect
10. **Outline** - Detects and draws edges using Sobel operator

#### Adjustment Filters
11. **Blur** - Gaussian blur with configurable kernel
12. **Contrast** - Adjustable contrast enhancement
13. **Saturation** - Color saturation control
14. **Warmth** - Adjusts color temperature
15. **Vignette** - Adds darkening effect around edges

#### Transformation Filters
16. **Horizontal Flip** - Mirrors image horizontally
17. **Vertical Flip** - Mirrors image vertically
18. **Rotate** - Rotates by 90, 180, or 270 degrees
19. **Resize** - Resize by dimensions or ratio (bilinear interpolation)
20. **Crop** - Crops to specified region
21. **Skew** - Horizontal skew transformation
22. **Wave** - Applies sine wave distortion

#### Composition Filters
23. **Merge** - Blends two images together
24. **Frame** - Adds decorative frame (2 built-in frames or custom)

## Project Structure

```
.
├── src/
│   ├── main.cpp                    # Entry point
│   ├── helpers/
│   │   ├── Image_Class.h           # Core image class using stb_image (Note: this file was provided as part of the assignment files)
│   │   ├── stb_image.h             # stb_image class (Note: this file was provided as part of the assignment files)
│   │   ├── stb_image_write.h       # stb_image_write class (Note: this file was provided as part of the assignment files)
│   │   ├── ImageProcessor.h        # Manages image state and undo/redo
│   │   └── Menu.h                  # Command-line interface
│   └── filters/
│       ├── Filter.h                # Base filter interface
│       ├── BlackAndWhiteFilter.h
│       ├── BlurFilter.h
│       ├── ContrastFilter.h
│       ├── CropFilter.h
│       ├── DarkenFilter.h
│       ├── FrameFilter.h
│       ├── GrayscaleFilter.h
│       ├── HorizontalFlipFilter.h
│       ├── InfraredFilter.h
│       ├── InvertFilter.h
│       ├── LightenFilter.h
│       ├── MergeFilter.h
│       ├── OilPaintingFilter.h
│       ├── OutlineFilter.h
│       ├── PurpleFilter.h
│       ├── ResizeFilter.h
│       ├── RetroFilter.h
│       ├── RotateFilter.h
│       ├── SaturationFilter.h
│       ├── SkewFilter.h
│       ├── VigentteFilter.h
│       ├── VerticalFlipFilter.h
│       ├── WarmthFilter.h
│       └── WaveFilter.h
├── assets/
│   └── frames/
│       ├── normal.jpg
│       └── fancy.jpg
├── Submissions/
│   ├── Image_Class.h               # Core image class using stb_image (Note: this file was provided as part of the assignment files)
│   ├── stb_image.h                 # stb_image class (Note: this file was provided as part of the assignment files)
│   ├── stb_image_write.h           # stb_image_write class (Note: this file was provided as part of the assignment files)
│   ├── A1.cpp                      # Our submission for Part 1
│   └── A2.cpp                      # Our submission for Part 2
├── LICENSE
├── .gitignore
└── README.md
```

## Building and Running

### Prerequisites
- C++17 or later compiler (g++, clang++)
- No external dependencies required (uses stb_image header library)

### Compilation
```bash
g++ -std=c++17 src/main.cpp -o photoshop
```

### Running
```bash
./photoshop
```

## Usage Guide

### Main Menu
```
1. Load a New Image       - Start by loading an image
2. Save Current Image     - Save your edited image
3. Apply Filter           - Choose from 22+ filters
4. Undo                   - Undo last filter
5. Redo                   - Redo last undone filter
0. Exit                   - Exit the application
```

### Workflow Example
1. Load an image: `input.jpg`
2. Apply Grayscale filter
3. Apply Blur filter
4. If needed, use Undo to revert
5. Save the result: `output.png`

### Filter Parameters

Some filters accept customizable parameters:
- **Blur**: Kernel size (default: 13), sigma (default: 2.0) (Not implemented yet to take user input)
- **Contrast**: Factor (default: 2.0)
- **Saturation**: Factor (default: 2.0)
- **Warmth**: Factor (default: 2.5)
- **Vignette**: Factor (default: 1.5)
- **Resize**: By ratio or specific dimensions
- **Rotate**: 90, 180, or 270 degrees
- **Skew**: Angle between -89 and 89 degrees

## Technical Details

### Filter Architecture
- Abstract base class `Filter` with virtual `apply()` method
- Each filter implements specific image transformation
- Filters can be chained sequentially
- Non-destructive editing with undo/redo support

### Key Algorithms
- **Gaussian Blur**: Separable 1D convolution for efficiency
- **Edge Detection**: Sobel operator for outline filter
- **Bilinear Interpolation**: Used in resize and skew filters
- **Adaptive Thresholding**: For black & white conversion
- **Oil Painting**: Intensity-based region quantization

## Error Handling

The application includes comprehensive error handling:
- Bounds checking for all pixel operations
- Input validation for filter parameters

## Known Limitations

1. **Memory Usage**: Large images may consume significant memory due to undo/redo history
2. **Rotation**: Only supports 90-degree increments
3. **Merge Filter**: Requires manual size adjustment if images differ
4. **Performance**: Some filters (blur, oil painting) may be slow on large images

## Future Enhancements

- [ ] GUI interface option

## References

- [stb_image Library](https://github.com/nothings/stb)
- [Gaussian Blur - Wikipedia](https://en.wikipedia.org/wiki/Gaussian_blur)
- [Sobel Operator - Wikipedia](https://en.wikipedia.org/wiki/Sobel_operator)
- [Bilinear Interpolation](https://en.wikipedia.org/wiki/Bilinear_interpolation)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Dr. Mohamed El-Ramely - Course Instructor
- Teaching Assistants - FCAI Cairo University
- Original Image_Class implementation by Shehab Diab, Youssef Mohamed, and Nada Ahmed
- Original stb_image and stb_image_write implementations provided during the assignment files

## Contributing

This is an academic project. For questions or suggestions, please contact the team members.

---

**Cairo University - Faculty of Computers and Artificial Intelligence**  
**CS213 - Object Oriented Programming - 2025**
