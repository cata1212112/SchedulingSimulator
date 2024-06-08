//
// Created by Cata on 3/17/2024.
//

#include <iostream>
#include "MinMaxRR.h"

vector<Event> MinMaxRR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push_back(process);
    }
    if (!p.empty()) {
        lastArrived = time;
    }
    return {};
}

vector<Event> MinMaxRR::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> MinMaxRR::schedule(int time, Metrics &stats, bool timerExpired) {
    if (time == lastArrived) {
        computeTimeQuant();
    }
    if (currentProcess == nullptr) {
        if (readyQueue->empty()) {
            return {};
        }
        cnt ++;
        return {assignToCPU(time, stats)};
    } else if (timerExpired){
        MinMaxRR::preemtCPU(stats, time);
        cnt ++;
        return {assignToCPU(time, stats)};
    }
    return {};
}

string MinMaxRR::getCoreAlgortihm(int coreID) {
    return "Min Max Round Robin";
}

MinMaxRR::MinMaxRR() {
    quant = 10;
    readyQueue = new vector<Process>();
}

void MinMaxRR::preemtCPU(Metrics &stats, int time) {
    currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - quant);
    currentProcess->setEnteredReadyQueue(time);
    readyQueue->push_back(*currentProcess);

    SchedulingAlgorithm::preemtCPU(stats, time);
}