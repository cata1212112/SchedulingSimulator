//
// Created by Cata on 3/11/2024.
//

#include <unordered_map>
#include <map>
#include <iostream>
#include "Core.h"
#include "../../Utils/ImplementedAlgorithms.h"

void Core::addEvent(Event e) {
    if (e.getType() == ARRIVAL) {
        lastArrived = coreTime;
    }
    events->push(e);
}

void Core::runSimulation() {
    unordered_map<eventType, vector<Event>> currentEvents;

    Metrics stats(algortihm, roundRobinQuant);
    stats.setCore(coreID);
    while (true) {
        {
            std::unique_lock lk(*cvMutex);
            cv->wait(lk, [this] { return *osTimeUpdated; });
        }

        while (!events->empty() && !finished) {
            Event e = events->top();
            if (e.getTime() > *osTime) {
                if (e.getType() == FINISHEXECUTION) {
                    finished = true;
                }
                break;
            }

            events->pop();

            coreTime = e.getTime();
            if (coreTime != e.getTime()) {
            }
            currentEvents.clear();

            currentEvents[e.getType()].push_back(e);

            while (!events->empty() && events->top().getTime() == coreTime) {
                e = events->top();
                currentEvents[e.getType()].push_back(e);
                events->pop();
            }

            if (!currentEvents[PREEMT].empty()) {
                priority_queue<Event> tmp;
                map<int, bool> toPreempt;
                for (const auto &event : currentEvents[PREEMT]) {
                    toPreempt[event.getProcess().getId()] = true;
                }
                while (!events->empty()) {
                    if (!toPreempt[events->top().getProcess().getId()]) {
                        tmp.push(events->top());
                    }
                    events->pop();
                }

                while (!tmp.empty()) {
                    events->push(tmp.top());
                    tmp.pop();
                }
                continue;
            }


            vector<Process> arrivedProcesses;
            vector<Process> ioCompleteProcesses;

            for (const auto& event : currentEvents[ARRIVAL]) {
                numberOfProcesses += 1;
                seenIds[event.getProcess().getId()] = 1;
                arrivedProcesses.push_back(event.getProcess());
            }

            for (const auto &event : currentEvents[IOBURSTCOMPLETE]) {
                ioCompleteProcesses.push_back(event.getProcess());
            }
            vector<Event> eventsGenerated;

            schedAlgo.processArrived(arrivedProcesses, coreTime, stats);
            schedAlgo.processIOComplete(ioCompleteProcesses, coreTime, stats);
            if (!currentEvents[CPUBURSTCOMPLETE].empty()) {
                eventsGenerated = schedAlgo.processCPUComplete(currentEvents[CPUBURSTCOMPLETE][0].getProcess(), coreTime, stats);
            }

            vector<Event> aux = schedAlgo.schedule(coreTime, stats, !currentEvents[TIMEREXPIRED].empty());
            eventsGenerated.insert(eventsGenerated.end(), aux.begin(), aux.end());

            for (const auto& eventGenerated : eventsGenerated) {
                events->push(eventGenerated);
            }
        }


        if (finished) {
            barrier->arrive_and_drop();
            break;
        } else {
            barrier->arrive_and_wait();
        }

    }
    stats.divide(coreTime, numberOfProcesses);
    p.set_value(stats);
    finished = true;
}

Core::Core(int *osTime, condition_variable *cv, mutex *cvMutex, string algorithm,
           bool *osTimeUpdated, std::barrier<> *barrier, int coreID, int roundRobinQuant)
        : osTime(osTime), cv(cv),cvMutex(cvMutex),roundRobinQuant(roundRobinQuant),barrier(barrier), algortihm(algorithm), osTimeUpdated(osTimeUpdated), coreID(coreID),   schedAlgo(ImplementedAlgorithms::getAlgorithm(algortihm, roundRobinQuant)){
    events = new priority_queue<Event>();
    schedAlgo.addMainEventQueue(events, nullptr);
    roundRobinQuant = 10;
    runningThread = new std::thread(&Core::runSimulation, this);
}

Metrics Core::join() {
    auto val = p.get_future();
    runningThread->join();
    return val.get();
}

int Core::getCoreTime() const {
    return coreTime;
}

bool Core::isFinished() const {
    return finished;
}

bool Core::isSentFinish() const {
    return sentFinish;
}

void Core::setSentFinish(bool sentFinish) {
    Core::sentFinish = sentFinish;
}

bool Core::running() {
    return schedAlgo.isRunning();
}

int Core::getCoreId() const {
    return coreID;
}
