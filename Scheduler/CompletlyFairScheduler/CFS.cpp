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
//        mainEventQueue->push(Event(TICK, time + load_balanicng_period + 1, Process()));
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
    double totalLoad = 0;
    map<int, int> coreLoads;
//
    for (const auto &c:cores) {
        int coreLoad = c->getLoad(time, true);
        coreLoads[c->getCoreId()] = coreLoad;
        totalLoad += coreLoad;
    }


    totalLoad /= cores.size();

    for (int i=0; i<cores.size(); i++) {
        while (coreLoads[cores[i]->getCoreId()] > totalLoad) {
            auto minTask = std::min_element(cores[i]->getReadyQueue()->begin(), cores[i]->getReadyQueue()->end(), [](const Process &a, const Process &b) {
                return prio_to_weight[a.getPriority()] < prio_to_weight[b.getPriority()];
            });

            if (minTask == cores[i]->getReadyQueue()->end()) {
                break;
            }

            int minCore = 0;
            for (int k=0; k<cores.size(); k++) {
                if (coreLoads[k] < coreLoads[minCore]) {
                    minCore = k;
                }
            }

            if (minCore == i) {
                break;
            }

            coreLoads[i] -= prio_to_weight[minTask->getPriority()];
            coreLoads[minCore] += prio_to_weight[minTask->getPriority()];

            auto it = std::find_if(cores[i]->getReadyQueue()->begin(), cores[i]->getReadyQueue()->end(),
                                   [&](const Process &a) {
                                       return a.getId() == minTask->getId();
                                   });

            if (it != cores[i]->getReadyQueue()->end()) {
                cores[i]->getReadyQueue()->erase(it);
            }

            cores[minCore]->addEvent(Event(ARRIVAL, time, *minTask));

            priority_queue<Event> tmp;

            while (!cores[i]->getEventQueue()->empty()) {
                tmp.push(cores[i]->getEventQueue()->top());
                cores[i]->getEventQueue()->pop();
            }

            while (!tmp.empty()) {
                if (tmp.top().getProcess().getId() != minTask->getId()) {
                    cores[i]->getEventQueue()->push(tmp.top());
                }
                tmp.pop();
            }
        }

        cores[i]->addEvent(Event(TICK, time, Process()));
    }

    return 0;
}

void CFS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
    mainEventQueue->push(Event(LOADBALANCE, load_balanicng_period, Process()));
}

void CFS::getMaximumVtimeDiff(Metrics &stats) {
    double dMax = 0;


    vector<double> vtimes[cores.size()];
    for (int i=0; i<cores.size(); i++) {
        vtimes[i] = getVtimesCore(i);
    }

    vector<double> allVtimes;
    for (int i=0; i<cores.size(); i++) {
        for (auto v:vtimes[i]) {
            allVtimes.push_back(v);
        }
    }

    for (auto v1:allVtimes) {
        for (auto v2:allVtimes) {
            dMax = max(dMax, abs(v1 - v2));
        }
    }

    stats.addDifference(dMax);
}

vector<double> CFS::getVtimesCore(int coreID) {
    return cores[coreID]->getVtimes();
}
