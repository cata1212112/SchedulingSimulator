//
// Created by cata_ on 3/2/2024.
//

#ifndef LICENTA_RR_H
#define LICENTA_RR_H

#include "../SchedulingAlgorithm.h"

class RR : public SchedulingAlgorithm{
private:
    int quant = 10;
    int ioQueue = 0;
    int lastArrived = 0;

    static std::function<bool(const Process&, const Process&)> RRQueue;

   vector<Process>* readyQueue;
public:
    int getQuant() const {
        return quant;
    }

    void setQuant(int quant) {
        RR::quant = quant;
    }

    RR();

    RR(int quant = 10) :quant(quant){
        readyQueue = new vector<Process>();
    }

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    string getCoreAlgortihm(int coreID) override;

    std::vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    Event assignToCPU(int time, Metrics &stats) {
        currentProcess = new Process(*readyQueue->begin());
        readyQueue->erase(readyQueue->begin());

        if (currentProcess->getRemainingBurst() <= quant) {
            assignProcessToCPU(*currentProcess, stats, time);
            return Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), *currentProcess);

        } else {
            assignProcessToCPU(*currentProcess, stats, time);
            return Event{TIMEREXPIRED, time + quant, *currentProcess};
        }
    }

    void preemtCPU(Metrics &stats, int time) override;
};


#endif //LICENTA_RR_H
