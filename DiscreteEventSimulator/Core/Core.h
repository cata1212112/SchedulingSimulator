//
// Created by Cata on 3/11/2024.
//

#ifndef LICENTA_CORE_H
#define LICENTA_CORE_H

#include <queue>
#include "../Event/Event.h"
#include "../../Utils/Metrics.h"
#include "../../Scheduler/SchedulingAlgorithm.h"
#include <thread>
#include <mutex>
#include <barrier>
#include <future>
#include <condition_variable>

class Core {
private:
    std::priority_queue<Event> *events = nullptr;
//    shared_ptr<priority_queue<Event>> events = nullptr;
    int coreTime = 0;
    int *osTime;
    std::condition_variable *cv;
    std::mutex *cvMutex;
    string algortihm;
    int roundRobinQuant;
    std::thread *runningThread;
    std::promise<Metrics> p;
    bool *osTimeUpdated;
    bool finished = false;
    std::barrier<> *barrier;
    int coreID;
    bool sentFinish = false;
    SchedulingAlgorithm &schedAlgo;
    int lastArrived = -1;
    int numberOfProcesses = 0;
    map<int, bool> seenIds;

public:
    int loadBalanceState;

    bool isSentFinish() const;

    void setSentFinish(bool sentFinish);

    Core(int *osTime, condition_variable *cv, mutex *cvMutex, string algorithm,
         bool *osTimeUpdated, std::barrier<> *barrier, int coreID, int roundRobinQuant = 10);

    void addEvent(Event e);
    void runSimulation();

    int getCoreId() const;

    Metrics join();

    int getCoreTime() const;

    bool isFinished() const;

    bool running();

    int getAbsoluteDeadline() {
        return schedAlgo.getAbsoluteDeadline();
    }

    int getLoad(int time, bool preemt = false) {
        return schedAlgo.getLoad(time, preemt);
    }

    vector<Process> *getReadyQueue() {
        return schedAlgo.getReadyQueue();
    }

    void addProcessIfNoTSeen(int id) {
        if (!seenIds[id]) {
            numberOfProcesses += 1;
            seenIds[id] = true;
        }
    }

    int getIsRunning() {
        return schedAlgo.isRunning();
    }

    priority_queue<Event> *getEventQueue();
};

#endif //LICENTA_CORE_H
