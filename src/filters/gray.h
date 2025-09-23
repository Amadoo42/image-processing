void gray(Image & a) {
    for(int i = 0; i < a.width; i++) {
        for(int j = 0; j < a.height; j++) {
            unsigned int avg = 0;
            for(int k = 0; k < 3; k++) avg += a(i, j, k);
            avg /= 3;
            for(int k = 0; k < 3; k++) a(i, j, k) = avg;
        }
    }
}