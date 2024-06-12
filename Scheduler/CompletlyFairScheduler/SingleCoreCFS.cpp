//
// Created by Cata on 3/11/2024.
//

#include "SingleCoreCFS.h"

vector<Event> SingleCoreCFS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue.push_back(process);
    }
    return {};
}

vector<Event> SingleCoreCFS::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

vector<Event> SingleCoreCFS::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr) {
        if (readyQueue.empty()) {
            return {};
        }
        return {assignToCPU(time, stats)};
    } else if (timerExpired){
        SingleCoreCFS::preemtCPU(stats, time);

        return {assignToCPU(time, stats)};
    }
    return {};
}

void SingleCoreCFS::preemtCPU(Metrics &stats, int time) {
    currentProcess->setPtime(currentProcess->getPtime() + currentProcess->getTimeSlice());
    currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - currentProcess->getTimeSlice());
    currentProcess->setEnteredReadyQueue(time);
    if (currentProcess->getRemainingBurst() > 0) {
        readyQueue.push_back(*currentProcess);
    }

    SchedulingAlgorithm::preemtCPU(stats, time);
}


int SingleCoreCFS::getSchedLatency() const {
    return sched_latency;
}

void SingleCoreCFS::setSchedLatency(int schedLatency) {
    sched_latency = schedLatency;
}

int SingleCoreCFS::getSchedNrLatency() const {
    return sched_nr_latency;
}

void SingleCoreCFS::setSchedNrLatency(int schedNrLatency) {
    sched_nr_latency = schedNrLatency;
}

int SingleCoreCFS::getSchedMinGranularity() const {
    return sched_min_granularity;
}

void SingleCoreCFS::setSchedMinGranularity(int schedMinGranularity) {
    sched_min_granularity = schedMinGranularity;
}

int SingleCoreCFS::getTimeSlice() {
    if (numProcs <= sched_nr_latency) {
        return sched_latency;
    }
    return numProcs * sched_min_granularity;
}

int SingleCoreCFS::getIsIdle() const {
    return isIdle;
}

long long int SingleCoreCFS::getLoad(int time, bool preemt) {
    long long int load = 0;
    Metrics workaroundStats("");

    if (preemt && currentProcess != nullptr) {
        eventQueue->push(Event(PREEMT, time, *currentProcess));
        SingleCoreCFS::preemtCPU(workaroundStats, time);
    }
    for (const auto &p:readyQueue) {
        load += prio_to_weight[p.getPriority()];
    }
    return load;
}

vector<Process> *SingleCoreCFS::getReadyQueue() {
    return &readyQueue;
}

string SingleCoreCFS::getCoreAlgortihm(int coreID) {
    return "SingleCoreCFS";
}

bool SingleCoreCFS::isRunning() {
    return (currentProcess != nullptr || (!readyQueue.empty()));
}

void SingleCoreCFS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    this->eventQueue = eventQueue;
}

vector<double> SingleCoreCFS::getVtimes() {
    vector<double> vtimes;
    if (currentProcess != nullptr) {
        vtimes.push_back(currentProcess->getPtime() * prio_to_weight[20] / prio_to_weight[currentProcess->getPriority()]);
    }

    for (const auto &p:readyQueue) {
        vtimes.push_back(p.getPtime() * prio_to_weight[20] / prio_to_weight[p.getPriority()]);
    }
    return vtimes;
}
