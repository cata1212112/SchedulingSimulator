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
#include "../Scheduler/EDRR/EDRR.h"
#include "../Scheduler/TestMultiCore1/TestMultiCore1.h"

class ImplementedAlgorithms {
public:
    static std::vector<std::string> getSingleCoreAlgorithms() {
        return {"First In First Out", "Shortest Job First", "Shortest Remaining Time First", "Round Robin", "Efficient Dynamic Round Robin"};
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
        } else if (name == "Efficient Dynamic Round Robin") {
            EDRR *edrr = new EDRR();
            return *edrr;
        } else if (name == "Combinatie FIFO SRTF(test 2 coruri)") {
            TestMultiCore1 *tmc1 = new TestMultiCore1();
            return *tmc1;
        }
    }

    static std::vector<std::string> getMultiCoreAlgortihms() {
        return {"Combinatie FIFO SRTF(test 2 coruri)", "Combinatie RR FIFO SJF SRTF(test 4 coruri)", "Completely Fair Scheduler"};
    }
};