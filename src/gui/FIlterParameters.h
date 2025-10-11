void applyGrayscale() {
    GrayscaleFilter filter;
    processor.applyFilter(filter);
    std::cout << "Grayscale filter applied." << std::endl;
}

void applyInvert() {
    InvertFilter filter;
    processor.applyFilter(filter);
    std::cout << "Invert filter applied." << std::endl;
}

void applyBlur() {
    BlurFilter filter;
    processor.applyFilter(filter);
    std::cout << "Blur filter applied." << std::endl;
}

void applyBlackAndWhite() {
    BlackAndWhiteFilter filter;
    processor.applyFilter(filter);
    std::cout << "Black & white filter applied." << std::endl;
}

//TODO : add parameters
void applyCrop() {
    CropFilter filter(0, 0, 500, 500);
    processor.applyFilter(filter);
    std::cout << "Crop filter applied." << std::endl;
}

void applyResize() {
    ResizeFilter filter(500, 500);
    processor.applyFilter(filter);
    std::cout << "Resize filter applied." << std::endl;
}

//TODO: ADD SLIDERS
void applyDarken() {
    DarkenFilter filter;
    processor.applyFilter(filter);
    std::cout << "Darken filter applied." << std::endl;
}

//TODO: ADD Sliders
void applyLighten() {
    LightenFilter filter;
    processor.applyFilter(filter);
    std::cout << "Lighten filter applied." << std::endl;
}

//TODO: add parameter here
void applyFrame() {
    Image frame_image("../assets/frames/fancy.png");
    FrameFilter filter(frame_image);
    processor.applyFilter(filter);
    std::cout << "Frame filter applied." << std::endl;
}

void applyHorizontalFlip() {
    HorizontalFlipFilter filter;
    processor.applyFilter(filter);
    std::cout << "Horizontal Flip filter applied." << std::endl;
}

void applyVerticalFlip() {
    VerticalFlipFilter filter;
    processor.applyFilter(filter);
    std::cout << "Vertical Flip filter applied." << std::endl;
}

void applyMerge() {
    Image merge_image("../assets/SampleImages/luffy.jpg");
    MergeFilter filter(merge_image);
    processor.applyFilter(filter);
    std::cout << "Merge filter applied." << std::endl;
}

void applyRotate() {
    RotateFilter filter(90);
    processor.applyFilter(filter);
    std::cout << "Rotate filter applied." << std::endl;
}

void applyOutline() {
    OutlineFilter filter;
    processor.applyFilter(filter);
    std::cout << "Outline filter applied." << std::endl;
}

void applyPurple() {
    PurpleFilter filter;
    processor.applyFilter(filter);
    std::cout << "Purple filter applied." << std::endl;
}

void applyInfrared(){
    InfraredFilter filter;
    processor.applyFilter(filter);
    std::cout << "Infrared filter applied." << std::endl;
}

void applyWave() {
    WaveFilter filter;
    processor.applyFilter(filter);
    std::cout << "Wave filter applied." << std::endl;
}

void applyOilPainting() {
    OilPaintingFilter filter;
    processor.applyFilter(filter);
    std::cout << "Oil painting filter applied." << std::endl;
}

void applyRetro() {
    RetroFilter filter;
    processor.applyFilter(filter);
    std::cout << "Retro filter applied." << std::endl;
}

void applySkew() {
    SkewFilter filter(45);
    processor.applyFilter(filter);
    std::cout << "Skew filter applied." << std::endl;
}

void applyWarmth() {
    WarmthFilter filter;
    processor.applyFilter(filter);
    std::cout << "Warmth filter applied." << std::endl;
}

void applySaturation() {
    SaturationFilter filter;
    processor.applyFilter(filter);
    std::cout << "Saturation filter applied." << std::endl;
}

void applyContrast() {
    ContrastFilter filter;
    processor.applyFilter(filter);
    std::cout << "Contrast filter applied." << std::endl;
}

void applyVigentte() {
    VigentteFilter filter;
    processor.applyFilter(filter);
    std::cout << "Vigentte filter applied." << std::endl;
}