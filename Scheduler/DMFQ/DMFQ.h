//
// Created by Cata on 3/17/2024.
//

#ifndef LICENTA_DMFQ_H
#define LICENTA_DMFQ_H

#include "../SchedulingAlgorithm.h"


class DMFQ : public SchedulingAlgorithm{
private:
    std::vector<Process>* Q[5];
public:
    DMFQ();

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    bool existsProcess();
};


#endif //LICENTA_DMFQ_H
