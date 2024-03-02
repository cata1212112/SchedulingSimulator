//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_IMPLEMENTEDALGORITHMS_H
#define LICENTA_IMPLEMENTEDALGORITHMS_H

#endif //LICENTA_IMPLEMENTEDALGORITHMS_H

#include <vector>
#include <string>
#include "../Scheduler/FIFO/FIFO.h"

class ImplementedAlgorithms {
public:
    static std::vector<std::string> getSingleCoreAlgorithms() {
        return {"First In First Out", "Shortest Job First", "Shortest Remaining Time First", "Round Robin"};
    }

    static SchedulingAlgorithm& getAlgorithm(const std::string &name) {
        FIFO *fifo = new FIFO();
        return *fifo;
    }
};