#include <stdio.h>

float array(float *x) {
    return x[3];
}

int main() {
    float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float result = array(a);
    printf("Result: %f\n", result);
    return 0;
}
