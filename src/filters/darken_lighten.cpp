#include "../helpers/apply_filter.h"

void Filter::darken(Image & a) {
    for(int i = 0; i < a.width; i++) {
        for(int j = 0; j < a.height; j++) {
            for(int k = 0; k < 3; k++) {
                a(i, j, k) = (a(i, j, k) + 0) / 2;
            }
        }
    }
}
void Filter::lighten(Image & a) {
    for(int i = 0; i < a.width; i++) {
        for(int j = 0; j < a.height; j++) {
            for(int k = 0; k < 3; k++) {
                a(i, j, k) = (a(i, j, k) + 255) / 2;
            }
        }
    }
}