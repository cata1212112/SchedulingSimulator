//
// Created by Cata on 3/15/2024.
//

#ifndef LICENTA_TESTMULTICORE1_H
#define LICENTA_TESTMULTICORE1_H

#include "../SchedulingAlgorithm.h"

class TestMultiCore1 : public SchedulingAlgorithm{
public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    int assignCPU(Process p) override;

    string getCoreAlgortihm(int coreID) override;
};


#endif //LICENTA_TESTMULTICORE1_H
