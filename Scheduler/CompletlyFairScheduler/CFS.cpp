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
        if (freeCore->getLoad(0) < cores[i]->getLoad(0)) {
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

void CFS::loadBalance(int time) {
    double totalLoad = 0;
    for (const auto &c:cores) {
        totalLoad += c->getLoad(time, true);
    }
    totalLoad /= cores.size();

    for (auto &c:cores) {
        if (c->getLoad(0) >= totalLoad) {
            int excessLoad = c->getLoad(0) - totalLoad;
            while (excessLoad > 0 && !c->getReadyQueue()->empty()) {
                Process task = *c->getReadyQueue()->begin();
                task.setEnteredReadyQueue(time);
                c->getReadyQueue()->erase(c->getReadyQueue()->begin());
                int targetCPU = leastLoadedCPU();

                excessLoad -= prio_to_weight[task.getPriority()];
                cores[targetCPU]->getReadyQueue()->push_back(task);
                cores[targetCPU]->addProcessIfNoTSeen(task.getId());
                cores[targetCPU]->addEvent(Event(TICK, time, Process()));
            }
        } else {
            int deficitLoad = totalLoad - c->getLoad(0);
            while (deficitLoad > 0) {
                int mostLoadedCPU_ = mostLoadedCPU();
                Process task = *cores[mostLoadedCPU_]->getReadyQueue()->begin();
                task.setEnteredReadyQueue(time);

                cores[mostLoadedCPU_]->getReadyQueue()->erase(cores[mostLoadedCPU_]->getReadyQueue()->begin());
                c->getReadyQueue()->push_back(task);
                c->addProcessIfNoTSeen(task.getId());
                c->addEvent(Event(TICK, time, Process()));
                deficitLoad -= prio_to_weight[task.getPriority()];
            }
        }
    }
}

int CFS::leastLoadedCPU() {
    int index = cores.size() - 1;
    for (int i=cores.size() - 1; i>=0; i--) {
        if (cores[i]->getLoad(0) < cores[index]->getLoad(0)) {
            index = i;
        }
    }
    return index;
}

int CFS::mostLoadedCPU() {
    int index = 0;
    for (int i=0; i<cores.size(); i++) {
        if (cores[i]->getLoad(0) > cores[index]->getLoad(0)) {
            index = i;
        }
    }
    return index;
}

void CFS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
}