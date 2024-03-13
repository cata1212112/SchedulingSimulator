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
#include <barrier>
#include <future>
#include <condition_variable>
#include <syncstream>

class Core {
private:
    std::priority_queue<Event> *events = nullptr;
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

public:
    Core(int *osTime, condition_variable *cv, mutex *cvMutex, string algorithm,
         bool *osTimeUpdated, std::barrier<> *barrier, int coreID, int roundRobinQuant = 0);

    void addEvent(Event e);
    void runSimulation();

    Metrics join();

    int getCoreTime() const;

    bool isFinished() const;
};


#endif //LICENTA_CORE_H
