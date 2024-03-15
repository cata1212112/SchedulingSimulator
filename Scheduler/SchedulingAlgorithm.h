//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_SCHEDULINGALGORITHM_H
#define CPUSCHED_SCHEDULINGALGORITHM_H

#include "../DiscreteEventSimulator/Event/Event.h"
#include <queue>
#include "../Utils/WrapperPriorityQueue.h"
#include "../Utils/Metrics.h"
#include <vector>

class SchedulingAlgorithm {
protected:
    Process* currentProcess = nullptr;
public:
    virtual std::vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats)=0;
    virtual std::vector<Event> processCPUComplete(Process p, int time, Metrics &stats)=0;
    virtual std::vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats)=0;
    virtual std::vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats)=0;
    virtual std::vector<Event> schedule(int time, Metrics &stats, bool timerExpired) =0;
    virtual int assignCPU(Process p) {return 0;}
    virtual string getCoreAlgortihm(int coreID)=0;

};

#endif //CPUSCHED_SCHEDULINGALGORITHM_H
