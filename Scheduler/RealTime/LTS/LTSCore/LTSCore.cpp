//
// Created by Cata on 5/21/2024.
//

#include "LTSCore.h"

vector<Event> LTSCore::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push(proc);
    }
    return {};
}

vector<Event> LTSCore::processCPUComplete(Process p, int time, Metrics &stats) {
    currentProcess = nullptr;
    return std::vector<Event>();
}

vector<Event> LTSCore::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LTSCore::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LTSCore::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->front());
        readyQueue->pop();
        stats.addToGanttChart(currentProcess->getId(), time, time + currentProcess->getRemainingBurst());
        return {Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), Process(*currentProcess->consumeBurst()))};
    }
    return {};
}

string LTSCore::getCoreAlgortihm(int coreID) {
    return std::string();
}

void LTSCore::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainQueue = eventQueue;
    queueMutex = m;
}

int LTSCore::assignCPU(Process p) {
    return SchedulingAlgorithm::assignCPU(p);
}

void LTSCore::addCore(void *core) {
    SchedulingAlgorithm::addCore(core);
}

bool LTSCore::isRunning() {
    if (currentProcess != nullptr) {
        return false;
    }
    return !readyQueue->empty();
}

int LTSCore::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

LTSCore::LTSCore() {
    readyQueue = new queue<Process>();
}