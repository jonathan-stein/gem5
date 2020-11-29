#include <stdio.h>

float addition(float x, float y) {
    return x + y;
}

int main() {
    float a = 1.3;
    float b = 3.4;
    float result = addition(a, b);
    printf("Result: %f\n", result);
    return 0;
}
