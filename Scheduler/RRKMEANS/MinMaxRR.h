//
// Created by Cata on 3/17/2024.
//

#ifndef LICENTA_MINMAXRR_H
#define LICENTA_MINMAXRR_H

#include <functional>
#include "../SchedulingAlgorithm.h"
#include "../../Utils/KMEANS/DataPoint.h"
#include "../../Utils/KMEANS/KMEANS.h"

class MinMaxRR : public SchedulingAlgorithm{
private:
    int quant;
    int lastArrived = 0;
    int cnt = 0;
    std::vector<Process>* readyQueue;
public:

    MinMaxRR();


    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    Event assignToCPU(int time, Metrics &stats) {

        currentProcess = new Process(*(readyQueue->begin()));
        readyQueue->erase(readyQueue->begin());

        cout << "########\n";
        cout << "########\n";
        cout << "La timpul " << time << " cuanta este " << quant << "\n";
        cout << "########\n";
        cout << "########\n";

        if (currentProcess->getRemainingBurst() <= quant) {
            assignProcessToCPU(*currentProcess, stats, time);
            return Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), *currentProcess);

        } else {
            assignProcessToCPU(*currentProcess, stats, time);
            return Event{TIMEREXPIRED, time + quant, *currentProcess};
        }
    }

    void preemtCPU(Metrics &stats, int time) override;

    void computeTimeQuant() {
        int maxBurstTime = -1;
        int minBurstTime = 10000;
        for (const auto &p:*readyQueue) {
            maxBurstTime = max(maxBurstTime, p.getRemainingBurst());
            minBurstTime = min(minBurstTime, p.getRemainingBurst());
        }

        if (maxBurstTime == minBurstTime) {
            quant = maxBurstTime;
        } else {
            quant = maxBurstTime - minBurstTime;
        }
    }
};

#endif //LICENTA_MINMAXRR_H
