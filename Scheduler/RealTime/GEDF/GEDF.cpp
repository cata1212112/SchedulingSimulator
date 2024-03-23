//
// Created by Cata on 3/22/2024.
//

#include "GEDF.h"

vector<Event> GEDF::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDF::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDF::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDF::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDF::schedule(int time, Metrics &stats, bool timerExpired) {
}

string GEDF::getCoreAlgortihm(int coreID) {
    return "GEDFCore";
}

GEDF::GEDF() {
    readyQueue = new vector<Process>();
}

int GEDF::assignCPU(Process p) {
    Core* freeCore = nullptr;
    for (auto core:cores) {
        if (!core->running()) {
            freeCore = core;
        }
    }
    if (freeCore != nullptr) {
        return freeCore->getCoreId();
    }

    freeCore = cores[0];
    for (auto core:cores) {
        if (core->getAbsoluteDeadline() > freeCore->getAbsoluteDeadline()) {
            freeCore = core;
        }
    }
    return freeCore->getCoreId();
}

void GEDF::addCore(void *core) {
    cores.push_back(reinterpret_cast<Core*>(core));
}
