//
// Created by Cata on 3/17/2024.
//

#ifndef LICENTA_MMRR_H
#define LICENTA_MMRR_H

#include "../SchedulingAlgorithm.h"
#include <algorithm>

class MMRR : public SchedulingAlgorithm{
private:
    int quant;
    int lastArrived = 0;
    int cnt = 0;
    std::vector<Process>* readyQueue;
public:

    MMRR();


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
        sort(readyQueue->begin(), readyQueue->end(), [](const Process &a, const Process &b) {
            return a.getRemainingBurst() < b.getRemainingBurst();
        });
        int sumOfBurst = 0;
        for (const auto &p:*readyQueue) {
            sumOfBurst += p.getRemainingBurst();
        }
        sumOfBurst /= readyQueue->size();
        quant = (sumOfBurst + (*readyQueue)[readyQueue->size() / 2].getRemainingBurst()) / 2;
    }
};


#endif //LICENTA_MMRR_H
