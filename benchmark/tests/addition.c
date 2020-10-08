#include <stdio.h>

float add(float x, float y) {
    return x + y;
}

int main() {
    float a = 1.3;
    float b = 3.4;
    float result = add(a, b);
    printf("%f\n", result);
    return 0;
}


