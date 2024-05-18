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
    if (timerExpired) {
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

int CFS::loadBalance(int time) {
//    cout << "Load Balance " << time << "\n";
    double totalLoad = 0;
    map<int, int> coreLoads;

    for (const auto &c:cores) {
        int coreLoad = c->getLoad(time, true);
        coreLoads[c->getCoreId()] = coreLoad;
        totalLoad += coreLoad;
    }
    totalLoad /= cores.size();
    int maximumDif = 0;
    for (int i=0; i<cores.size(); i++) {
        for (int j=0; j<cores.size(); j++) {
            maximumDif = max(maximumDif, abs(coreLoads[cores[i]->getCoreId()] - coreLoads[cores[j]->getCoreId()]));
        }
    }

    auto leastLoadedCPU = [&](){
        int indexMin = 0;
        for (int i=1; i<cores.size(); i++) {
            if (coreLoads[cores[i]->getCoreId()] < coreLoads[cores[indexMin]->getCoreId()]) {
                indexMin = i;
            }
        }
        return indexMin;
    };

    auto mostLoadedCPU = [&](){
        int indexMin = 0;
        for (int i=1; i<cores.size(); i++) {
            if (coreLoads[cores[i]->getCoreId()] > coreLoads[cores[indexMin]->getCoreId()]) {
                indexMin = i;
            }
        }
        return indexMin;
    };

    for (auto &c:cores) {
        int coreLoad = coreLoads[c->getCoreId()];
        if (coreLoad > totalLoad) {
            int excessLoad = coreLoad - totalLoad;
            while (excessLoad > 0 && !c->getReadyQueue()->empty()) {
                Process task = Process(*c->getReadyQueue()->begin());
                c->getReadyQueue()->erase(c->getReadyQueue()->begin());


                int targetCPU = leastLoadedCPU();

                excessLoad -= prio_to_weight[task.getPriority()];
                coreLoads[cores[targetCPU]->getCoreId()] += prio_to_weight[task.getPriority()];
                cores[targetCPU]->addEvent(Event(ARRIVAL, time, task));
//                cores[targetCPU]->getReadyQueue()->push_back(task);
//                cores[targetCPU]->addProcessIfNoTSeen(task.getId());
            }
        } else if (coreLoad < totalLoad){
            int deficitLoad = totalLoad - coreLoad;
            while (deficitLoad > 0) {
                int mostLoadedCPU_ = mostLoadedCPU();
                if (cores[mostLoadedCPU_]->getReadyQueue()->empty()) {
                    deficitLoad = 0;
                    break;
                }
                Process task = Process(*cores[mostLoadedCPU_]->getReadyQueue()->begin());

                cores[mostLoadedCPU_]->getReadyQueue()->erase(cores[mostLoadedCPU_]->getReadyQueue()->begin());
//                c->getReadyQueue()->push_back(task);
//                c->addProcessIfNoTSeen(task.getId());
                c->addEvent(Event(ARRIVAL, time, task));
                deficitLoad -= prio_to_weight[task.getPriority()];
                coreLoads[c->getCoreId()] += prio_to_weight[task.getPriority()];
            }
        }
    }

    return maximumDif;
}

void CFS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
    mainEventQueue->push(Event(LOADBALANCE, load_balanicng_period, Process()));
}