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
#include <iostream>

class SchedulingAlgorithm {
protected:
    Process* currentProcess = nullptr;
public:
    int coreID;
    int lastProc = -1;
    virtual std::vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats)=0;
    virtual std::vector<Event> schedule(int time, Metrics &stats, bool timerExpired) =0;
    virtual std::vector<Event> processCPUComplete(Process p, int time, Metrics &stats) {
        stats.addToGantt("Left: " + to_string(currentProcess->getLastStarted()) + " Right: " + to_string(time) + " " + "Proc: " +
                         to_string(currentProcess->getId()) + "\n");
        stats.addToCPUUtilization(time - currentProcess->getLastStarted());
        currentProcess = nullptr;
        stats.addToTT(time - p.getArrivalTime(), p.getDistributionId());

        return {};
    };

    void assignProcessToCPU(Process p, Metrics &stats, int time) {
        currentProcess = new Process(p);
        if (lastProc != -1) {
            if (p.getId() != lastProc) {
                stats.incrementCS();
            }
        }

        lastProc = p.getId();
        if (!currentProcess->getAssigned()) {
            stats.addToRT(time - currentProcess->getArrivalTime(), currentProcess->getDistributionId());
            currentProcess->setAssigned(true);
        }
        currentProcess->setLastStarted(time);
        stats.addToWT(time - currentProcess->getEnteredReadyQueue(), currentProcess->getDistributionId());
    }

    virtual void preemtCPU(Metrics &stats, int time) {
        stats.addToGantt("Left: " + to_string(currentProcess->getLastStarted()) + " Right: " + to_string(time) + " " + "Proc: " +
                         to_string(currentProcess->getId()) + "\n");
        stats.addToCPUUtilization(time - currentProcess->getLastStarted());
        currentProcess = nullptr;
    }

    virtual std::vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
        return {};
    };
    virtual std::vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) {
        return {};
    };
    virtual int assignCPU(Process p) {return 0;}
    virtual string getCoreAlgortihm(int coreID)=0;

    virtual void addCore(void* core) {
        return;
    };

    virtual void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {

    }

    virtual bool isRunning() {
        return currentProcess != nullptr;
    }

    virtual int getAbsoluteDeadline() {
        if (currentProcess == nullptr) {
            return 0;
        }
        return currentProcess->getAbsoluteDeadline();
    }

    virtual long long int getLoad(int time, bool preemt) {
        return 0;
    }

    virtual vector<Process> *getReadyQueue() {
        return nullptr;
    }

    virtual int loadBalance(int time) {
        return 0;
    };

    virtual void getMaximumVtimeDiff(Metrics &stats) {

    }

    virtual vector<double> getVtimes() {
        return {};
    }

    Process *getCurrentProcess() const {
        return currentProcess;
    }
};

#endif //CPUSCHED_SCHEDULINGALGORITHM_H
