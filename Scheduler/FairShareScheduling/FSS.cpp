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
//        mainEventQueue->push(Event(TICK, time + load_balanicng_period + 1, Process()));
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

    vector<vector<Process>> taskGroups;
    int cnt = 0;
    for (const auto &c:cores) {
        int coreLoad = c->getLoad(time, true);
    }


    for (auto &c:cores){
        vector<Process> group;
        map<int, bool> seenIds;

        for (auto &p:*c->getReadyQueue()) {
            group.push_back(Process(p));
            cnt += 1;
            seenIds[p.getId()] = true;
        }
        if (!c->getReadyQueue()->empty()) {
            c->getReadyQueue()->clear();
        }

        priority_queue<Event> tmp;

        while (!c->getEventQueue()->empty()) {
            if (c->getEventQueue()->top().getType() == TICK || !seenIds[c->getEventQueue()->top().getProcess().getId()]) {
                tmp.push(c->getEventQueue()->top());
            }
            c->getEventQueue()->pop();
        }

        while (!tmp.empty()) {
            c->getEventQueue()->push(tmp.top());
            tmp.pop();
        }


        taskGroups.push_back(group);
    }

    if (cnt > 0) {
        auto pb = progressBalancing(cores.size(), taskGroups);


        for (int i=0; i<pb.size(); i++) {
            for (auto &p:pb[i].first) {
                cores[i]->addEvent(Event(ARRIVAL, time, Process(p)));
            }
            Process auxProc = Process();
            auxProc.setId(-1);
            auxProc.setPriority(pb[i].second);
//            cout << pb[i].second << "\n";
//            cores[i]->addEvent(Event(ARRIVAL, time, auxProc));
        }
    }


    return 0;

}

void FSS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
    mainEventQueue->push(Event(LOADBALANCE, load_balanicng_period, Process()));
}

vector<pair<vector<Process>, double>> FSS::progressBalancing(int numCores, vector<vector<Process>> taskGroups) {
    vector<Process> S;
    double totalWeigth = 0;
    int wmin = 1e9;
    int wmax = 0;

    for (auto &t:taskGroups) {
        for (auto &p:t) {
            S.push_back(p);
            totalWeigth += prio_to_weight[p.getPriority()];
            wmin = min(wmin, prio_to_weight[p.getPriority()]);
            wmax = max(wmax, prio_to_weight[p.getPriority()]);
        }
    }
    cout << (load_balanicng_period + 0.0) / wmin * 100 << "\n";
//    cout << wmin << "\n";
//    cout << wmax << "\n";
    vector<int> toDelete;
    for (int i=0; i<S.size(); i++) {
        if (prio_to_weight[S[i].getPriority()] > totalWeigth / numCores) {
            toDelete.push_back(i);
            totalWeigth -= prio_to_weight[S[i].getPriority()];
        }
    }
    for (auto it = toDelete.rbegin(); it != toDelete.rend(); it++) {
        S.erase(S.begin() + *it);
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
    if (numCores == 0) {
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

//    while (u <= w / 2 && S.size() > 1) {
    while (u < w / 2) {
        Process task = Process(*S.begin());
        S.erase(S.begin());
        u += prio_to_weight[task.getPriority()];
        left.push_back(task);
    }
    for (auto t:S) {
        right.push_back(Process(t));
    }

    int loadLeft = 0;
    int loadRight = 0;

    double leftMin = 1e9;
    double rightMax = 0;

    for (auto p:left) {
        loadLeft += prio_to_weight[p.getPriority()];
        leftMin = min(leftMin, p.getVtime());
    }
    for (auto p:right) {
        loadRight += prio_to_weight[p.getPriority()];
        rightMax = max(rightMax, p.getVtime());

    }
//    cout << "Left min: " << leftMin << " Right max " << rightMax << "\n";
//    cout << "Load left: " << loadLeft << " Load right: " << loadRight << " Diff: " << loadLeft - loadRight << "\n";
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

    for (int j=taskGroups.size() - 1; j >= 1; j--) {
        if ((loads[j-1] + 0.0) >= (loads[j] + 0.0) / throttleFactors[j]) {
            continue;
        }
        throttleFactors[j-1] = throttleFactors[j] * (loads[j-1] + 0.0) / (loads[j] + 0.0);
    }
//    cout << "Throttle Factors: \n";
//    for (auto x:throttleFactors) {
//        cout << x << " ";
//    }
//    cout << "\n";
    return throttleFactors;
}

void FSS::getMaximumVtimeDiff(Metrics &stats) {
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

vector<double> FSS::getVtimesCore(int coreID) {
    return cores[coreID]->getVtimes();
}
