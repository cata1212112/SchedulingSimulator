//
// Created by Cata on 3/22/2024.
//

#include "GEDFCore.h"

vector<Event> GEDFCore::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push(proc);
    }
    return {};
}

vector<Event> GEDFCore::processCPUComplete(Process p, int time, Metrics &stats) {
    currentProcess = nullptr;
    return {};
}

vector<Event> GEDFCore::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDFCore::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDFCore::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->front());
        readyQueue->pop();
        stats.addToGanttChart(currentProcess->getId(), time, time + currentProcess->getRemainingBurst());
        return {Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), Process(*currentProcess->consumeBurst()))};
    }
    return {};
}

int GEDFCore::assignCPU(Process p) {
    return SchedulingAlgorithm::assignCPU(p);
}

string GEDFCore::getCoreAlgortihm(int coreID) {
    return std::string();
}

GEDFCore::GEDFCore() {
    readyQueue = new queue<Process>();
}

bool GEDFCore::isRunning() {
    if (currentProcess != nullptr) {
        return false;
    }
    return !readyQueue->empty();
}

int GEDFCore::getAbsoluteDeadline() {
    if (currentProcess != nullptr) {
        return currentProcess->getAbsoluteDeadline();
    }

    return readyQueue->front().getAbsoluteDeadline();
}
