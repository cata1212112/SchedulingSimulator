//
// Created by Cata on 5/21/2024.
//

#include "DARTSCore.h"

vector<Event> DARTSCore::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push(proc);
    }
    return {};
}

vector<Event> DARTSCore::processCPUComplete(Process p, int time, Metrics &stats) {
    currentProcess = nullptr;
    return std::vector<Event>();
}

vector<Event> DARTSCore::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> DARTSCore::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> DARTSCore::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->front());
        readyQueue->pop();
        stats.addToGanttChart(currentProcess->getId(), time, time + currentProcess->getRemainingBurst());
        return {Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), Process(*currentProcess->consumeBurst()))};
    }
    return {};
}

string DARTSCore::getCoreAlgortihm(int coreID) {
    return std::string();
}

void DARTSCore::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainQueue = eventQueue;
    queueMutex = m;
}

int DARTSCore::assignCPU(Process p) {
    return SchedulingAlgorithm::assignCPU(p);
}

void DARTSCore::addCore(void *core) {
    SchedulingAlgorithm::addCore(core);
}

bool DARTSCore::isRunning() {
    if (currentProcess != nullptr) {
        return false;
    }
    return !readyQueue->empty();
}

int DARTSCore::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

DARTSCore::DARTSCore() {
    readyQueue = new queue<Process>();
}