//
// Created by Cata on 3/17/2024.
//

#include "MMRR.h"

vector<Event> MMRR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push_back(process);
    }
    if (!p.empty()) {
        lastArrived = time;
    }
    return {};
}

vector<Event> MMRR::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> MMRR::schedule(int time, Metrics &stats, bool timerExpired) {
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
        MMRR::preemtCPU(stats, time);
        cnt ++;
        return {assignToCPU(time, stats)};
    }
    return {};
}

string MMRR::getCoreAlgortihm(int coreID) {
    return "Mean Median Time Quantum Round Robin";
}

MMRR::MMRR() {
    quant = 10;
    readyQueue = new vector<Process>();
}

void MMRR::preemtCPU(Metrics &stats, int time) {
    currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - quant);
    currentProcess->setEnteredReadyQueue(time);
    readyQueue->push_back(*currentProcess);

    SchedulingAlgorithm::preemtCPU(stats, time);
}