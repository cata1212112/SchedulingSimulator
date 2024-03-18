//
// Created by Cata on 3/17/2024.
//

#ifndef LICENTA_TESTMULTICORE2_H
#define LICENTA_TESTMULTICORE2_H

#include "../SchedulingAlgorithm.h"

class TestMultiCore2 : public SchedulingAlgorithm{
public:
    TestMultiCore2();

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    int assignCPU(Process p) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;
};


#endif //LICENTA_TESTMULTICORE2_H
