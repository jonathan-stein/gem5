#include <stdio.h>

float branch(float x) {
    if (x < 1.0f) {
        return 0.5f;
    } else {
        return 1.0f;
    }
}

int main() {
    float result = branch(0.5f);
    printf("Result: %f\n", result);
    return 0;
}
