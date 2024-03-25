//
// Created by Cata on 3/24/2024.
//

#include "LSTRCore.h"

vector<Event> LSTRCore::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push(proc);
    }
    return {};
}

vector<Event> LSTRCore::processCPUComplete(Process p, int time, Metrics &stats) {
    currentProcess = nullptr;
    return std::vector<Event>();
}

vector<Event> LSTRCore::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LSTRCore::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LSTRCore::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->front());
        readyQueue->pop();
        stats.addToGanttChart(currentProcess->getId(), time, time + currentProcess->getRemainingBurst());
        return {Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), Process(*currentProcess->consumeBurst()))};
    }
    return {};
}

string LSTRCore::getCoreAlgortihm(int coreID) {
    return std::string();
}

void LSTRCore::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainQueue = eventQueue;
    queueMutex = m;
}

int LSTRCore::assignCPU(Process p) {
    return SchedulingAlgorithm::assignCPU(p);
}

void LSTRCore::addCore(void *core) {
    SchedulingAlgorithm::addCore(core);
}

bool LSTRCore::isRunning() {
    if (currentProcess != nullptr) {
        return false;
    }
    return !readyQueue->empty();
}

int LSTRCore::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

LSTRCore::LSTRCore() {
    readyQueue = new queue<Process>();
}
