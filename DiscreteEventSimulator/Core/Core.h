//
// Created by Cata on 3/11/2024.
//

#ifndef LICENTA_CORE_H
#define LICENTA_CORE_H

#include <queue>
#include "../Event/Event.h"
#include "../../Utils/Metrics.h"
#include <thread>
#include <mutex>
#include <future>

#include <condition_variable>

class Core {
private:
    std::priority_queue<Event> *events = nullptr;
    int coreTime = 0;
    int *osTime;
    std::condition_variable *cv;
    std::condition_variable *cvNumThreads;
    std::mutex *cvMutex;
    std::mutex *threadsNumMutex;
    bool *continueExecution;
    string algortihm;
    int roundRobinQuant;
    std::thread *runningThread;
    std::promise<Metrics> p;
    bool cantContinue = false;
    bool *osTimeUpdated;
    int *numThreadsFinished;
    std::mutex *updatedMutex;
    std::condition_variable *updatedCV;
    bool finished = false;

public:
    Core(int *osTime, condition_variable *cv, condition_variable *cvNumThreads, mutex *cvMutex,mutex *updatedMutex,condition_variable *updatedCV,bool *continueExecution, string algorithm, bool *osTimeUpdated, int *numThreadsFinished, std::mutex *threadsNumMutex, int roundRobinQuant = 0);

    void addEvent(Event e);
    void runSimulation();

    Metrics join();

    int getCoreTime() const;

    bool isCantContinue() const;

    bool isFinished() const;
};


#endif //LICENTA_CORE_H
