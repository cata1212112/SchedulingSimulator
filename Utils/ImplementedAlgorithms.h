//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_IMPLEMENTEDALGORITHMS_H
#define LICENTA_IMPLEMENTEDALGORITHMS_H

#endif //LICENTA_IMPLEMENTEDALGORITHMS_H

#include <vector>
#include <string>
#include "../Scheduler/FIFO/FIFO.h"
#include "../Scheduler/SJF/SJF.h"
#include "../Scheduler/RR/RR.h"
#include "../Scheduler/SRTF/SRTF.h"

class ImplementedAlgorithms {
public:
    static std::vector<std::string> getSingleCoreAlgorithms() {
        return {"First In First Out", "Shortest Job First", "Shortest Remaining Time First", "Round Robin"};
    }

    static SchedulingAlgorithm& getAlgorithm(const std::string &name, int quant=-1) {
        if (name == "First In First Out") {
            FIFO *fifo = new FIFO();
            return *fifo;
        } else if (name == "Shortest Job First") {
            SJF *sjf = new SJF();
            return *sjf;
        } else if (name == "Shortest Remaining Time First") {
            SRTF *srtf = new SRTF();
            return *srtf;
        } else if (name == "Round Robin") {
            RR *rr = new RR(quant);
            return *rr;
        }
    }
};