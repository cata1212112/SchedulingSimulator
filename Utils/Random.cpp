//
// Created by Cata on 2/12/2024.
//

#include "Random.h"

std::random_device Random::rd;
std::mt19937 Random::gen(Random::rd());
std::bernoulli_distribution Random::bernoulli(0.5);
std::exponential_distribution<> Random::exponential(0.5);
std::uniform_int_distribution<> Random::uniform(0, 300);

bool Random::randomBit() {
    return bernoulli(gen);
}

int Random::randomBurst(double multiplier) {
    int randomInteger = exponential(gen);
    return randomInteger * multiplier;
}

int Random::randomInteger(int max, int min) {
    if (min != uniform.a() || max != uniform.b()) {
        uniform = std::uniform_int_distribution<>(min, max);
    }
    return uniform(gen);
}
