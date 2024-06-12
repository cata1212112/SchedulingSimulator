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
#include <functional>
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
    std::barrier<> *secondBarrier;

    std::barrier<> *firstCoresBarrier;
    std::barrier<> *secondCoresBarrier;
    std::mutex *m;
    int *cateSunt;
    vector<int> *minimums;
    priority_queue<Event> *DESevents;

    bool finished = false;
    std::barrier<> *barrier;
    int coreID;
    bool sentFinish = false;
    SchedulingAlgorithm &schedAlgo;
    int lastArrived = -1;
    int numberOfProcesses = 0;
    map<int, bool> seenIds;
    bool isRealTime = false;
    int hyperPeriod;
    int numCPUs;

public:
    int getHyperPeriod() const;

    void setHyperPeriod(int hyperPeriod);

    int loadBalanceState;

    bool isSentFinish() const;

    void setSentFinish(bool sentFinish);

    Core(int numCPUs, int *osTime, condition_variable *cv, mutex *cvMutex, string algorithm,
         bool *osTimeUpdated, std::barrier<> *barrier, std::barrier<> *secondBarrier,int coreID,
         std::barrier<> *firstCoresBarrier, std::barrier<> *secondCoresBarrier, mutex *m, int *cateSunt, vector<int> *minimums, priority_queue<Event> *DESevents,
         int roundRobinQuant = 10);

    void setIsRealTime(bool isRealTime);

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

    vector<double> getVtimes() {
        return schedAlgo.getVtimes();
    }

    SchedulingAlgorithm &getSchedAlgo() const;
};

#endif //LICENTA_CORE_H
