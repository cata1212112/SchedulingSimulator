//
// Created by Cata on 3/11/2024.
//

#include "SingleCoreCFS.h"

vector<Event> SingleCoreCFS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        isIdle = false;
        process.setEnteredReadyQueue(time);
        readyQueue.push_back(process);
        numProcs += 1;
    }
    return {};
}

vector<Event> SingleCoreCFS::processCPUComplete(Process p, int time, Metrics &stats) {
    stats.addToGanttChart(p.getId(), p.getLastStarted(), time);
    stats.addToCPUUtilization(time - p.getLastStarted());
    numProcs -= 1;
    currentProcess = nullptr;
    std::vector<Event> events;
    if (p.hasRemainingIO()) {
        events.push_back(Event(IOBURSTCOMPLETE, time + p.getRemainingBurst(), Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

vector<Event> SingleCoreCFS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
        process.setEnteredReadyQueue(time);
        addToReadyQueue.push_back(process);
    }
    for (auto p:addToReadyQueue) {
        readyQueue.push_back(p);
    }
    return {};
}

vector<Event> SingleCoreCFS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> SingleCoreCFS::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && readyQueue.empty()) {
        isIdle = true;
        return {};
    }
    if (timerExpired || currentProcess == nullptr) {
        isIdle = false;
        if (currentProcess != nullptr) {
            currentProcess->setVtime(
                    ((time - currentProcess->getLastStarted()) / prio_to_weight[currentProcess->getPriority()]));
            readyQueue.push_back(*currentProcess);
            currentProcess = nullptr;
        }

        int minIndex = 0;
        for (int i = 0; i < readyQueue.size(); i++) {
            if (readyQueue[i].getVtime() < readyQueue[minIndex].getVtime()) {
                minIndex = i;
            }
        }

        int processTimeSlice = ((getTimeSlice() * prio_to_weight[readyQueue[minIndex].getPriority()]) / getLoad());
        currentProcess = new Process(readyQueue[minIndex]);
        currentProcess->setLastStarted(time);
        cout << currentProcess->getRemainingBurst() << " " << processTimeSlice << " " << currentProcess->getId() << "\n";
        readyQueue.erase(readyQueue.begin() + minIndex);
        int remainingBurst = currentProcess->getRemainingBurst();

        if (processTimeSlice >= currentProcess->getRemainingBurst()) {
            return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
        }
        currentProcess->setVtime(currentProcess->getVtime() +
                                 ((processTimeSlice / prio_to_weight[readyQueue[minIndex].getPriority()])));
        currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - processTimeSlice);
        return {Event{TIMEREXPIRED, time + processTimeSlice, Process(*currentProcess)}};
    }
    return {};
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

int SingleCoreCFS::getLoad() {
    int load = 0;
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
