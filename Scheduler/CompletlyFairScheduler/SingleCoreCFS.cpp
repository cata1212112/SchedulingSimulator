//
// Created by Cata on 3/11/2024.
//

#include "SingleCoreCFS.h"

vector<Event> SingleCoreCFS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        isIdle = false;
        if (process.getId() < 0) {
            cout << "ok";
        }
        process.setEnteredReadyQueue(time);
        readyQueue.push_back(process);
        numProcs += 1;
    }
    return {};
}

vector<Event> SingleCoreCFS::processCPUComplete(Process p, int time, Metrics &stats) {
    stats.addToGanttChart(p.getId(), p.getLastStarted(), time);
    stats.addToCPUUtilization(time - p.getLastStarted());
    stats.addToTT(time - p.getArrivalTime());
    numProcs -= 1;
    currentProcess = nullptr;
    return {};
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
    workaroundStats = &stats;
    if (currentProcess == nullptr && readyQueue.empty()) {
        isIdle = true;
        return {};
    }
    if (timerExpired || currentProcess == nullptr) {
        isIdle = false;
        if (currentProcess != nullptr) {
            currentProcess->setVtime(currentProcess->getVtime() +
                                     ((time - currentProcess->getLastStarted() + 0.0) * prio_to_weight[0] / prio_to_weight[currentProcess->getPriority()]));
            currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted()));
            currentProcess->setEnteredReadyQueue(time);
            stats.addToGanttChart(currentProcess->getId(), currentProcess->getLastStarted(), time);
            stats.addToCPUUtilization(time - currentProcess->getLastStarted());
            if (currentProcess->getRemainingBurst() > 0) {
                readyQueue.push_back(Process(*currentProcess));
            }
            currentProcess = nullptr;
        }

        int minIndex = 0;
        for (int i = 0; i < readyQueue.size(); i++) {
            if (readyQueue[i].getVtime() < readyQueue[minIndex].getVtime()) {
                minIndex = i;
            }
        }

        int processTimeSlice = ((0.0 + getTimeSlice() * (prio_to_weight[readyQueue[minIndex].getPriority()] + 0.0) / (getLoad(time) + 0.0)));
        processTimeSlice = max(processTimeSlice, sched_min_granularity);
        currentProcess = new Process(readyQueue[minIndex]);
        currentProcess->setLastStarted(time);
        if (!currentProcess->getAssigned()) {
            stats.addToRT(time - currentProcess->getArrivalTime());
            currentProcess->setAssigned(true);
        }
        readyQueue.erase(readyQueue.begin() + minIndex);
        int remainingBurst = currentProcess->getRemainingBurst();
//
//        if (time + min(processTimeSlice, currentProcess->getRemainingBurst()) > throttleMaixmum) {
//            processTimeSlice = throttleMaixmum - time;
//        }
//        if (processTimeSlice <= 0) {
//            return {};
//        }

        if (processTimeSlice >= currentProcess->getRemainingBurst()) {
            return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess))};
        }
        stats.addToWT(time - currentProcess->getEnteredReadyQueue());
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

long long int SingleCoreCFS::getLoad(int time, bool preemt) {
    long long int load = 0;
    if (preemt && currentProcess != nullptr) {

        currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted()));
        currentProcess->setEnteredReadyQueue(time);
        currentProcess->setVtime(currentProcess->getVtime() + (time - currentProcess->getLastStarted() + 0.0) / prio_to_weight[currentProcess->getPriority()]);

        if (workaroundStats != nullptr) {
            workaroundStats->addToGanttChart(currentProcess->getId(), currentProcess->getLastStarted(), time);
            workaroundStats->addToCPUUtilization(time - currentProcess->getLastStarted());
        }

        priority_queue<Event> tmp;

        while (!eventQueue->empty()) {
            if (eventQueue->top().getType() == TICK || eventQueue->top().getProcess().getId() != currentProcess->getId()) {
                tmp.push(eventQueue->top());
            }
            eventQueue->pop();
        }

        while (!tmp.empty()) {
            eventQueue->push(tmp.top());
            tmp.pop();
        }

        if (currentProcess->getRemainingBurst() > 0) {
            load += prio_to_weight[currentProcess->getPriority()];
            eventQueue->push(Event(ARRIVAL, time, Process(*currentProcess)));
//            cout << coreID << ":: " << currentProcess->getId() << " " << currentProcess->getLastStarted() << " " << time << "\n";

        } else {
//            cout << "Finish " << coreID << ":: " << currentProcess->getId() << " " << currentProcess->getLastStarted() << " " << time << "\n";
        }
        currentProcess = nullptr;
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
        vtimes.push_back(currentProcess->getVtime());
    }

    for (const auto &p:readyQueue) {
        vtimes.push_back(p.getVtime());
    }
    return vtimes;
}
