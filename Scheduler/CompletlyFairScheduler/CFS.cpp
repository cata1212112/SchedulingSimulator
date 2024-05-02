//
// Created by Cata on 3/10/2024.
//

#include "CFS.h"

vector<Event> CFS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> CFS::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> CFS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> CFS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> CFS::schedule(int time, Metrics &stats, bool timerExpired) {
    return std::vector<Event>();
}

string CFS::getCoreAlgortihm(int coreID) {
    return std::string();
}

int CFS::assignCPU(Process p) {
    return SchedulingAlgorithm::assignCPU(p);
}

void CFS::addCore(void *core) {
    SchedulingAlgorithm::addCore(core);
}

void CFS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    SchedulingAlgorithm::addMainEventQueue(eventQueue, m);
}

bool CFS::isRunning() {
    return SchedulingAlgorithm::isRunning();
}

int CFS::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}
