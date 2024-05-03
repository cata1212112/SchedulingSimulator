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
    if (time == 0 || timerExpired) {
        mainEventQueue->push(Event(LOADBALANCE, time + load_balanicng_period, Process()));
    }
    return {};
}

string CFS::getCoreAlgortihm(int coreID) {
    return "SingleCoreCFS";
}

int CFS::assignCPU(Process p) {
    Core* freeCore = cores[0];
    for (int i=1; i<cores.size(); i++) {
        if (freeCore->getLoad() < cores[i]->getLoad()) {
            freeCore = cores[i];
        }
    }
    return freeCore->getCoreId();
}

void CFS::addCore(void *core) {
    cores.push_back(reinterpret_cast<Core*>(core));
}

bool CFS::isRunning() {
    return SchedulingAlgorithm::isRunning();
}

int CFS::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

int CFS::getSchedLatency() const {
    return sched_latency;
}

void CFS::setSchedLatency(int schedLatency) {
    sched_latency = schedLatency;
}

int CFS::getSchedNrLatency() const {
    return sched_nr_latency;
}

void CFS::setSchedNrLatency(int schedNrLatency) {
    sched_nr_latency = schedNrLatency;
}

int CFS::getSchedMinGranularity() const {
    return sched_min_granularity;
}

void CFS::setSchedMinGranularity(int schedMinGranularity) {
    sched_min_granularity = schedMinGranularity;
}

void CFS::loadBalance() {
    double totalLoad = 0;
    for (const auto &c:cores) {
        totalLoad += c->getLoad();
    }
    totalLoad /= cores.size();

    for (auto &c:cores) {
        if (c->getLoad() >= totalLoad) {
            int excessLoad = c->getLoad() - totalLoad;
            while (excessLoad > 0 && !c->getReadyQueue()->empty()) {
                Process task = *c->getReadyQueue()->begin();
                c->getReadyQueue()->erase(c->getReadyQueue()->begin());
                int targetCPU = leastLoadedCPU();

                excessLoad -= prio_to_weight[task.getPriority()];
                cores[targetCPU]->getReadyQueue()->push_back(task);
            }
        } else {
            int deficitLoad = totalLoad - c->getLoad();
            while (deficitLoad > 0) {
                int mostLoadedCPU_ = mostLoadedCPU();
                Process task = *cores[mostLoadedCPU_]->getReadyQueue()->begin();
                cores[mostLoadedCPU_]->getReadyQueue()->erase(cores[mostLoadedCPU_]->getReadyQueue()->begin());
                c->getReadyQueue()->push_back(task);
                deficitLoad -= prio_to_weight[task.getPriority()];
            }
        }
    }
}

int CFS::leastLoadedCPU() {
    int index = 0;
    for (int i=0; i<cores.size(); i++) {
        if (cores[i]->getLoad() < cores[index]->getLoad()) {
            index = i;
        }
    }
    return index;
}

int CFS::mostLoadedCPU() {
    int index = 0;
    for (int i=0; i<cores.size(); i++) {
        if (cores[i]->getLoad() > cores[index]->getLoad()) {
            index = i;
        }
    }
    return index;
}

void CFS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
}