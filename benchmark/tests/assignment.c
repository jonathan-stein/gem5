#include <stdio.h>

float assignment() {
    const float x = 0.003f;
    return x;
}

int main() {
    float result = assignment();
    printf("Result: %f\n", result);
    return 0;
}
