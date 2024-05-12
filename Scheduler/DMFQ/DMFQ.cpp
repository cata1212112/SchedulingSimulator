//
// Created by Cata on 3/17/2024.
//

#include <iostream>
#include "DMFQ.h"

vector<Event> DMFQ::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue.push_back(process);
    }
    return {};
}

vector<Event> DMFQ::processCPUComplete(Process p, int time, Metrics &stats) {
    stats.addToGanttChart(p.getId(), p.getLastStarted(), time);
    stats.addToCPUUtilization(time - p.getLastStarted());

    currentProcess = nullptr;
    std::vector<Event> events;
    if (p.hasRemainingIO()) {
        int remainingTime = p.getRemainingBurst();
        events.push_back(Event(IOBURSTCOMPLETE, time + remainingTime, Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

vector<Event> DMFQ::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
        if (process.finished()) {
            stats.addToTT(time - process.getArrivalTime());
        } else {
            process.setEnteredReadyQueue(time);
            addToReadyQueue.push_back(process);
        }
    }
    for (auto p:addToReadyQueue) {
        Q[p.getPriority() / 2]->push_back(p);
    }
    return {};
}

vector<Event> DMFQ::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> DMFQ::schedule(int time, Metrics &stats, bool timerExpired) {
    if ((currentProcess == nullptr || timerExpired) && existsProcess()) {
        if (existsProcess()) {
            pair<int, pair<int,int>> aux = getNextProcess(time);
            int queueNum = aux.first;
            int proc = aux.second.first;
            int quant = aux.second.second;

            currentProcess = new Process(Q[queueNum]->at(proc));
            stats.incrementCS();
            currentProcess->setLastStarted(time);


            if (!currentProcess->getAssigned()) {
                stats.addToRT(time - currentProcess->getArrivalTime());
                currentProcess->setAssigned(true);
            }

            int remainingBurst = currentProcess->getRemainingBurst();

            stats.addToWT(time - currentProcess->getEnteredReadyQueue());

            if (remainingBurst > quant) {
                Q[queueNum]->at(proc).setRemainingBurst(Q[queueNum]->at(proc).getRemainingBurst() - quant);
                stats.addToGanttChart(currentProcess->getId(), time, time+quant);
                Q[queueNum]->at(proc).setEnteredReadyQueue(time+quant);
                if (queueNum < 4) {
                    Q[queueNum]->at(proc).setPriority(Q[queueNum]->at(proc).getPriority() + 2);
                    Process p = Q[queueNum]->at(proc);
                    Q[queueNum]->erase(Q[queueNum]->begin() + proc);
                    Q[queueNum+1]->push_back(p);
                }
                return {Event{TIMEREXPIRED, time + quant, Process(*currentProcess)}};
            }
            Q[queueNum]->erase(Q[queueNum]->begin() + proc);
            processNumber -= 1;
            return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};


        }
    }
    return {};
}

string DMFQ::getCoreAlgortihm(int coreID) {
    return "Developed Multilevel Feedback Queue Scheduling";
}

DMFQ::DMFQ() {
    for (int i=0; i<5; i++) {
        Q[i] = new vector<Process>();
        queuesUpdated[i] = false;
    }

}

bool DMFQ::existsProcess() {
    bool exists = false;
    for (int i=0; i<5; i++) {
        exists = (exists || !Q[i]->empty());
    }
    return exists || !readyQueue.empty();
}

int DMFQ::medianBurst(int queueId) {
    int sum = 0;
    for (const auto &p:*Q[queueId]) {
        sum += p.getRemainingBurst();
    }
    return sum / Q[queueId]->size() * (queueId + 1);
}

void DMFQ::manageQueuesAndQuants(int time) {
    for (auto &process:readyQueue) {
        Q[process.getPriority() / 2]->push_back(process);
        queuesUpdated[process.getPriority() / 2] = true;
    }
    readyQueue.clear();
    vector<Process> allProcesses;
    map<int, int> assignedQueue;
    for (int i=0; i<5; i++) {
        for (auto &process : *Q[i]) {
            allProcesses.push_back(process);
            assignedQueue[process.getId()] = i;
        }
    }
    for (int i=max(queueNumber+1, 0); i<4; i++) {
        for (auto &process : allProcesses) {
            int waitingTime = time - process.getEnteredReadyQueue();
            if ((waitingTime + 0.0) / process.getRemainingBurst() >= 1 && process.getPriority() / 2 == i + 1) {
                auto hasId = [&process](Process p) {return p.getId() == process.getId();};
                Q[assignedQueue[process.getId()]]->erase(std::find_if(Q[assignedQueue[process.getId()]]->begin(), Q[assignedQueue[process.getId()]]->end(), hasId));
                if (Q[assignedQueue[process.getId()]]->empty()) {
                    queuesUpdated[assignedQueue[process.getId()]] = false;
                }
                Q[i]->push_back(process);
                queuesUpdated[i] = true;
            }
        }
    }

    for (int i=0; i<5; i++) {
        if (queuesUpdated[i]) {
            sort(Q[i]->begin(), Q[i]->end(), [](const Process &a, const Process &b) {return a.getRemainingBurst() < b.getRemainingBurst();});
        }
    }
    for (int i=0; i<5; i++) {
        if (queuesUpdated[i]) {
            int sum = 0;
            for (const auto &p:*Q[i]) {
                sum += p.getRemainingBurst();
            }
            quants[i] = medianBurst(i);
            queuesUpdated[i] = false;
        }
    }
    for (int i=0; i<5; i++) {
        if (!Q[i]->empty()) {
            quants[i] = medianBurst(i);
        }
    }
}

pair<int, pair<int,int>> DMFQ::getNextProcess(int time) {

    if (queueNumber == -1) {
        manageQueuesAndQuants(time);
        processNumber = 0;
        queueNumber = 0;
    }

    if (processNumber >= Q[queueNumber]->size() || updateQueuesCheck()) {
        manageQueuesAndQuants(time);
        processNumber = 0;
    }

    if (processNumber >= Q[queueNumber]->size()) {
        processNumber = 0;
        if (Q[queueNumber]->empty()) {
            queueNumber += 1;
            if (queueNumber == 5) {
                queueNumber = 0;
            }
        }
    }
    while (Q[queueNumber]->empty()) {
        queueNumber += 1;
        if (queueNumber == 5) {
            queueNumber = 0;
        }
    }
    processNumber += 1;
    return {queueNumber, {processNumber - 1, quants[queueNumber]}};
}

bool DMFQ::updateQueuesCheck() {
    bool exists = false;
    for (int i=0; i<5; i++) {
        exists = (exists || !Q[i]->empty());
    }
    return !exists && !readyQueue.empty();
}
