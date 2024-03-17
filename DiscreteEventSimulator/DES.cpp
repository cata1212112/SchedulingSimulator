//
// Created by Cata on 2/12/2024.
//

#include "DES.h"
#include "Core/Core.h"
#include <map>
#include <iostream>
#include <ranges>

vector<string> DES::algortihms;

string DES::generateInputData(int numProcesses, int maximumTime=MAXIMUMARRIVAL) {

    events = new priority_queue<Event>();

    numberOfProcesses = numProcesses;

    string inputData;
    for (int i=0; i<numProcesses; i++) {
        int arrival = Random::randomInteger(maximumTime);
        int numBursts = Random::randomInteger(MAXIMUMNUMCPUBURSTS) + 1;
        bool processType = Random::randomBit();
        processType = true;
        int priority = Random::randomInteger(10);
        numBursts = 1;
        vector<int> cpuBursts;
        vector<int> ioBursts;

        for (int j=0; j<numBursts; j++) {
            int cpuBurst = Random::randomBurst(getMultiplier(processType));
            int ioBurst = Random::randomBurst(getMultiplier(!processType));

            cpuBursts.push_back(cpuBurst);
            if (j < numBursts - 1) {
                ioBursts.push_back(ioBurst);
            }
        }

        Process p(cpuBursts, ioBursts, priority);

        p.setArrivalTime(arrival);

        Event event = Event(ARRIVAL, arrival, p);

        inputData += event.toString();

        events->push(event);
    }
    input = inputData;
    return inputData;
}

void DES::readInputDataFromFile(const string& filename) {
    usedFileAsInput = true;
    events = new priority_queue<Event>();

    ifstream in(filename);

    stringstream buffer;
    buffer << in.rdbuf();

    input = buffer.str();

    setInputFromString(input);

    in.close();
}

vector<Metrics> DES::startSimulation(int numCPUS) {

    SchedulingAlgorithm &schedAlgo = ImplementedAlgorithms::getAlgorithm(algorithm, roundRobinQuant);

    int osTime = 0;
    int currentTime = 0;


    Core *core[numCPUS];

    std::barrier barrier(numCPUS + 1);

    for (int i=0; i<numCPUS; i++) {
        core[i] = new Core(&osTime, &cv, &cvMutex, schedAlgo.getCoreAlgortihm(i), &osTimeUpdated,&barrier, i, roundRobinQuant);
    }

    vector<Event> currentEvents;

    bool allFinished = true;

    while (true) {
        if (events->empty()) {
            allFinished = true;
            for (int i=0; i<numCPUS; i++) {
                if (!core[i]->isSentFinish()) {
                    core[i]->addEvent(Event(FINISHEXECUTION, 10000000, Process()));
                    core[i]->setSentFinish(true);
                }
                allFinished = (allFinished && core[i]->isFinished());
            }

            if (allFinished) {
                break;
            }

            osTime = 1000000;
        } else {
            Event e = events->top();
            events->pop();
            currentTime = e.getTime();

            currentEvents.clear();

            currentEvents.push_back(e);

            while (!events->empty() && events->top().getTime() == currentTime) {
                e = events->top();
                currentEvents.push_back(e);
                events->pop();
            }

            for (const auto &event : currentEvents) {
                if (event.getType() == ARRIVAL) {
                    core[schedAlgo.assignCPU(event.getProcess())]->addEvent(event);
                }
            }

            osTime = currentTime;
        }

        {
            std::lock_guard lk(cvMutex);
            osTimeUpdated = true;
        }
        cv.notify_all();

        barrier.arrive_and_wait();
        {
            std::lock_guard lk(cvMutex);
            osTimeUpdated = false;
        }

    }
    vector<Metrics> vec;

    for (int i=0; i<numCPUS; i++) {
        vec.push_back(core[i]->join());
    }
    return vec;

}

bool DES::isUsedFileAsInput() const {
    return usedFileAsInput;
}

string DES::getAlgorithm() const {
    if (algorithm == "Round Robin") {
        return algorithm + " TQ = " + to_string(roundRobinQuant);
    }
    return algorithm;
}

const string &DES::getInput() const {
    return input;
}

void DES::setInput(const string &input) {
    DES::input = input;
}

void DES::setAlgorithm(const string &algorithm) {
    algortihms.push_back(algorithm);
    DES::algorithm = algorithm;
}

void DES::setInputFromString(const string &input) {
    stringstream ss(input);
    events = new priority_queue<Event>();

    string firstLine, secondLine, thirdLine;
    vector<Event> eventVec;
    while (getline(ss, firstLine) && getline(ss, secondLine) && getline(ss, thirdLine)) {
        if (!firstLine.empty()) {
            numberOfProcesses += 1;
            Event event = Event::fromStrings(firstLine, secondLine, thirdLine);
//            eventVec.push_back(event);
            events->push(event);

        }
    }
//
//    for (auto & it : std::ranges::reverse_view(eventVec)) {
//        events->push(it);
//    }
}

const string &DES::getPartialMetricsInput(int core) const {
    return partialMetricsInput[core];
}

void DES::setPartialMetricsInput(const string &partialMetricsInput, int core) {
    DES::partialMetricsInput[core] = partialMetricsInput;
}

int DES::getRoundRobinQuant() const {
    return roundRobinQuant;
}

void DES::setRoundRobinQuant(int roundRobinQuant) {
    DES::roundRobinQuant = roundRobinQuant;
}
