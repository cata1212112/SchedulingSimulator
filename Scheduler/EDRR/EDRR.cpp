//
// Created by Cata on 3/16/2024.
//

#include <iostream>
#include "EDRR.h"

vector<Event> EDRR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push_back(process);
    }
    if (!p.empty()) {
        lastArrived = time;
    }
    return {};
}

vector<Event> EDRR::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> EDRR::schedule(int time, Metrics &stats, bool timerExpired) {
    if (time == lastArrived) {
        int maximumBurstTime = -1;
        for (const auto &p:*readyQueue) {
            maximumBurstTime = max(maximumBurstTime, p.getRemainingBurst());
        }

        quant = int(0.8 * maximumBurstTime);
    }
    if (currentProcess == nullptr) {
        if (readyQueue->empty()) {
            return {};
        }
        return {assignToCPU(time, stats)};
    } else if (timerExpired){
        EDRR::preemtCPU(stats, time);

        return {assignToCPU(time, stats)};
    }
    return {};
}

string EDRR::getCoreAlgortihm(int coreID) {
    return "Efficient Dynamic Round Robin";
}

EDRR::EDRR() {
    quant = 10;
    readyQueue = new vector<Process>();
}

void EDRR::preemtCPU(Metrics &stats, int time) {
    currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - quant);
    currentProcess->setEnteredReadyQueue(time);
    readyQueue->push_back(*currentProcess);

    SchedulingAlgorithm::preemtCPU(stats, time);
}