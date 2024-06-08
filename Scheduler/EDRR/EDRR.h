//
// Created by Cata on 3/16/2024.
//

#ifndef LICENTA_EDRR_H
#define LICENTA_EDRR_H

#include "../SchedulingAlgorithm.h"
#include <functional>
#include <queue>

class EDRR : public SchedulingAlgorithm{
private:
    int quant;
    int lastArrived = 0;

    std::vector<Process>* readyQueue;
public:
    EDRR();

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    Event assignToCPU(int time, Metrics &stats) {
        int position = -1;
        for (int i=0; i<readyQueue->size(); i++) {
            if ((*readyQueue)[i].getRemainingBurst() <= quant) {
                position = i;
                break;
            }
        }
        if (position == -1) {
            int maximumBurstTime = -1;
            for (const auto &p:*readyQueue) {
                maximumBurstTime = max(maximumBurstTime, p.getRemainingBurst());
            }
            quant = maximumBurstTime;
            position = 0;
        }

        currentProcess = new Process(*(readyQueue->begin() + position));
        readyQueue->erase(readyQueue->begin() + position);

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
};


#endif //LICENTA_EDRR_H
