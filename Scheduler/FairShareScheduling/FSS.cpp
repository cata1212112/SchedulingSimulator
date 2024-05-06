//
// Created by cata_ on 5/6/2024.
//

#include <iostream>
#include "FSS.h"

vector<Event> FSS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> FSS::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> FSS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> FSS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> FSS::schedule(int time, Metrics &stats, bool timerExpired) {
    if (timerExpired) {
        mainEventQueue->push(Event(LOADBALANCE, time + load_balanicng_period, Process()));
    }
    return {};
}

string FSS::getCoreAlgortihm(int coreID) {
    return "FSSCore";
}

int FSS::assignCPU(Process p) {
    Core* freeCore = cores[0];
    for (int i=1; i<cores.size(); i++) {
        if (freeCore->getLoad(0) < cores[i]->getLoad(0)) {
            freeCore = cores[i];
        }
    }
    return freeCore->getCoreId();
}

void FSS::addCore(void *core) {
    cores.push_back(reinterpret_cast<Core*>(core));
}

bool FSS::isRunning() {
    return SchedulingAlgorithm::isRunning();
}

int FSS::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

int FSS::getSchedLatency() const {
    return sched_latency;
}

void FSS::setSchedLatency(int schedLatency) {
    sched_latency = schedLatency;
}

int FSS::getSchedNrLatency() const {
    return sched_nr_latency;
}

void FSS::setSchedNrLatency(int schedNrLatency) {
    sched_nr_latency = schedNrLatency;
}

int FSS::getSchedMinGranularity() const {
    return sched_min_granularity;
}

void FSS::setSchedMinGranularity(int schedMinGranularity) {
    sched_min_granularity = schedMinGranularity;
}

int FSS::loadBalance(int time) {
    cout << "Load Balance " << time << "\n";
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

void FSS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
    mainEventQueue->push(Event(LOADBALANCE, load_balanicng_period, Process()));
}

vector<pair<vector<Process>, double>> FSS::progressBalancing(int numCores, vector<vector<Process>> taskGroups) {
    vector<Process> S;
    for (auto &t:taskGroups) {
        for (auto &p:t) {
            S.push_back(p);
        }
    }
    sort(S.begin(), S.end(), [](Process a, Process b) {
        return a.getVtime() > b.getVtime();
    });

    int l = log2(numCores);
    vector<vector<Process>> groups = partition(l, S);
    vector<double> throttleFactors = throttle(groups);

    vector<pair<vector<Process>, double>> aux;
    for (int i=0; i<groups.size(); i++) {
        aux.push_back({groups[i], throttleFactors[i]});
    }
    return aux;
}

vector<vector<Process>> FSS::partition(int numCores, vector<Process> S) {
    vector<vector<Process>> p;
    if (numCores == 1) {
        p.push_back(S);
        return p;
    }
    pair<vector<Process>, vector<Process>> tmp = binaryPartition(S);
    vector<vector<Process>> left = partition(numCores - 1, tmp.first);
    vector<vector<Process>> right = partition(numCores - 1, tmp.second);
    for (auto l:left) {
        p.push_back(l);
    }
    for (auto r:right) {
        p.push_back(r);
    }
    return p;
}

pair<vector<Process>, vector<Process>> FSS::binaryPartition(vector<Process> S) {
    vector<Process> left;
    vector<Process> right;

    int u = 0;
    int w = 0;
    for (auto &p:S) {
        w += prio_to_weight[p.getPriority()];
    }

    while (u <= w / 2) {
        Process task = Process(*S.begin());
        S.erase(S.begin());
        u += prio_to_weight[task.getPriority()];
        left.push_back(task);
    }
    for (auto t:S) {
        right.push_back(Process(t));
    }

    return {left, right};
}

vector<double> FSS::throttle(vector<vector<Process>> taskGroups) {
    vector<double> throttleFactors;
    for (int i=0; i<taskGroups.size(); i++) {
        throttleFactors.push_back(1.0);
    }
    vector<int> loads;
    for (auto &t:taskGroups) {
        int load = 0;
        for (auto &p:t) {
            load += prio_to_weight[p.getPriority()];
        }
        loads.push_back(load);
    }

    for (int j=taskGroups.size() - 1; j >= 2; j--) {
        if ((loads[j-1] + 0.0) >= (loads[j] + 0.0) / throttleFactors[j]) {
            continue;
        }
        throttleFactors[j-1] = throttleFactors[j] * (loads[j-1] + 0.0) / (loads[j] + 0.0);
    }
    return throttleFactors;
}
