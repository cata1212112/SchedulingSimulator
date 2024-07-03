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
    schedAlgo.addMainEventQueue(events, nullptr);
    unordered_map<eventType, vector<Event>> currentEvents;

    Metrics stats(algortihm, roundRobinQuant);
    stats.setCore(coreID);
    schedAlgo.coreID = getCoreId();
    int whereEntered = 0;

    while (true) {
        {
            std::unique_lock lk(*cvMutex);
            cv->wait(lk, [this] { return *osTimeUpdated; });
        }

        if (!events->empty()) {
            Event e = events->top();

            if (e.getTime() > *osTime) {
                if (e.getType() == FINISHEXECUTION) {
                    finished = true;
                }
            } else {
                events->pop();
                if (e.getTime() > 0) {
                    coreTime = e.getTime();
                }
                currentEvents[ARRIVAL] = {};
                currentEvents[CPUBURSTCOMPLETE] = {};
                currentEvents[IOBURSTCOMPLETE] = {};
                currentEvents[TIMEREXPIRED] = {};
                currentEvents[PREEMT] = {};
                currentEvents[FINISHEXECUTION] = {};
                currentEvents[REALTIME] = {};
                currentEvents[TICK] = {};
                currentEvents[LOADBALANCE] = {};

                currentEvents[e.getType()].push_back(e);

                while (!events->empty() && events->top().getTime() == coreTime) {
                    e = events->top();
                    if (e.getType() == LOADBALANCE) {
                        loadBalanceState = true;
                    }
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

                    for (auto& [key, events] : currentEvents) {
                        auto it = std::remove_if(events.begin(), events.end(), [&](const Event &e) {
                            return toPreempt[e.getProcess().getId()];
                        });

                        events.erase(it, events.end());
                    }

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

                auto newEvents = schedAlgo.processArrived(arrivedProcesses, coreTime, stats);

                schedAlgo.processIOComplete(ioCompleteProcesses, coreTime, stats);
                if (!currentEvents[CPUBURSTCOMPLETE].empty()) {
                    eventsGenerated = schedAlgo.processCPUComplete(currentEvents[CPUBURSTCOMPLETE][0].getProcess(), coreTime, stats);
                }

                vector<Event> aux = schedAlgo.schedule(coreTime, stats, !currentEvents[TIMEREXPIRED].empty());
                eventsGenerated.insert(eventsGenerated.end(), aux.begin(), aux.end());
                eventsGenerated.insert(eventsGenerated.end(), newEvents.begin(), newEvents.end());

                for (const auto& eventGenerated : eventsGenerated) {
                    events->push(eventGenerated);
                }
            }
        }


        if (events->empty()) {
            (*minimums)[coreID] = 1e9;
        } else {
            (*minimums)[coreID] = events->top().getTime();
        }

        if (finished) {
            firstCoresBarrier->arrive_and_drop();
        } else {
            firstCoresBarrier->arrive_and_wait();
        }

        {
            lock_guard lk(*m);
//            cout << "Core time " << coreTime << "\n";

            int timpulMinim = 1e9;
            if (*cateSunt == 0) {
                for (auto it:*minimums) {
//                        cout << it << " ";
                    if (it != 0) {
                        timpulMinim = min(it, timpulMinim);
                    }
                }

//                cout << "\n";
                if (timpulMinim != 1e9) {
                    DESevents->push(Event(IDLE, timpulMinim, Process()));
                }
            }
            *cateSunt = *cateSunt + 1;
            *cateSunt = *cateSunt % numCPUs;
        }

        if (finished) {
            secondCoresBarrier->arrive_and_drop();
        } else {
            secondCoresBarrier->arrive_and_wait();
        }

        (*minimums)[coreID] = 1e9;

        ///

        if (finished) {
            barrier->arrive_and_drop();
            secondBarrier->arrive_and_drop();
            break;
        } else {
            barrier->arrive_and_wait();
            secondBarrier->arrive_and_wait();
        }
    }
    stats.divide(coreTime, numberOfProcesses);
    p.set_value(stats);
    finished = true;
}

Core::Core(int numCPUs, int *osTime, condition_variable *cv, mutex *cvMutex, string algorithm,
           bool *osTimeUpdated, std::barrier<> *barrier, std::barrier<> *secondBarrier,int coreID, std::barrier<> *firstCoresBarrier, std::barrier<> *secondCoresBarrier, mutex *m, int *cateSunt, vector<int> *minimums, priority_queue<Event> *DESevents,
           int roundRobinQuant)
        : numCPUs(numCPUs), osTime(osTime), cv(cv),cvMutex(cvMutex),roundRobinQuant(roundRobinQuant),barrier(barrier), secondBarrier(secondBarrier),algortihm(algorithm), osTimeUpdated(osTimeUpdated),
          coreID(coreID),   schedAlgo(ImplementedAlgorithms::getAlgorithm(algortihm, roundRobinQuant)), firstCoresBarrier(firstCoresBarrier), secondCoresBarrier(secondCoresBarrier), m(m), cateSunt(cateSunt), minimums(minimums), DESevents(DESevents){
    roundRobinQuant = 10;
    events = new priority_queue<Event>();
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

priority_queue<Event> *Core::getEventQueue() {
    return events;
}

SchedulingAlgorithm &Core::getSchedAlgo() const {
    return schedAlgo;
}

void Core::setIsRealTime(bool isRealTime) {
    Core::isRealTime = isRealTime;
}

int Core::getHyperPeriod() const {
    return hyperPeriod;
}

void Core::setHyperPeriod(int hyperPeriod) {
    Core::hyperPeriod = hyperPeriod;
}