#include "helper.h"

int getRandomInt(int min, int max) {
    static std::random_device rd;     // seed source
    static std::mt19937 gen(rd());    // Mersenne Twister RNG
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}
