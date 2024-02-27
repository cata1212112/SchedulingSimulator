//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_RANDOM_H
#define CPUSCHED_RANDOM_H

#include <random>


class Random {
private:
    static std::random_device  rd;
    static std::mt19937 gen;
    static std::bernoulli_distribution bernoulli;
    static std::exponential_distribution<> exponential;
    static std::uniform_int_distribution<> uniform;

public:

    static int randomInteger(int max, int min=0);

    static int randomBurst(double multiplier);

    static bool randomBit();
};


#endif //CPUSCHED_RANDOM_H
