//
// Created by Cata on 2/12/2024.
//

#include "DES.h"
#include "Core/Core.h"
#include "../Utils/UUniFastDiscard.h"
#include <map>
#include <iostream>
#include <ranges>

vector<string> DES::algortihms;

string DES::generateInputData(vector<int> numProcesses, int maximumTime, vector<int> mean, vector<int> std) {

    events = new priority_queue<Event>();
    string inputData;

    for (auto x:generatedBurstByDistributionId) {
        generatedBurstByDistributionId[x.first].clear();
    }

    numberOfProcesses = 0;
    for (int i=0; i<numProcesses.size(); i++) {
        numberOfProcesses += numProcesses[i];

        generatedBurstByDistributionMean[i] = mean[i];
        generatedBurstByDistributionSTD[i] = std[i];

        for (int k=0; k<numProcesses[i]; k++) {
            Random::setGaussian(mean[i], std[i]);

            int arrival = Random::randomInteger(maximumTime);
            int priority = Random::randomInteger(8) + 1;

            int cpuBurst = Random::randomGaussian(1);

            generatedBurstByDistributionId[i].push_back(cpuBurst);

            Process p({cpuBurst}, {}, priority);

            p.setDistributionId(i);

            p.setArrivalTime(arrival);

            Event event = Event(ARRIVAL, arrival, p);

            inputData += event.toString();

            events->push(event);
        }
    }
    input = inputData;
    return inputData;
}

void DES::readInputDataFromFile(const string& filename, bool realTime) {
    usedFileAsInput = true;
    events = new priority_queue<Event>();

    ifstream in(filename);

    stringstream buffer;
    buffer << in.rdbuf();

    input = buffer.str();

    setInputFromString(input, realTime);

    in.close();
}

vector<Metrics> DES::startSimulation(int numCPUS) {
    SchedulingAlgorithm &schedAlgo = ImplementedAlgorithms::getAlgorithm(algorithm, roundRobinQuant);

    int osTime = 0;
    int currentTime = 0;
    bool loadBalance = false;

    Metrics multicoreFairness(algorithm);
    Core *core[numCPUS];

    std::barrier barrier(numCPUS + 1);
    std::barrier secondBarrier(numCPUS + 1);

    for (int i=0; i<numCPUS; i++) {
        core[i] = new Core(&osTime, &cv, &cvMutex, schedAlgo.getCoreAlgortihm(i), &osTimeUpdated,&barrier,&secondBarrier, i, roundRobinQuant);
        if (isRealTime()) {
            core[i]->setIsRealTime(true);
        }
        schedAlgo.addCore(core[i]);
    }
    schedAlgo.addMainEventQueue(events, nullptr);
    vector<Event> currentEvents;
    bool allFinished = false;
    Metrics realTimeMetrics("");
//    cout << toStop << "\n";
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
            if (events->empty() && e.getType() == LOADBALANCE) {
                allFinished = true;
                for (int i=0; i<numCPUS; i++) {
                    allFinished = (allFinished && !core[i]->getIsRunning());
                }
            }
            if (isRealTime() && e.getTime() >= toStop) {
                while (!events->empty()) {
                    events->pop();
                }
            } else {
                currentTime = e.getTime();

                currentEvents.clear();

                currentEvents.push_back(e);
                bool timerExpired = false;
                while (!events->empty() && events->top().getTime() == currentTime) {
                    e = events->top();
                    currentEvents.push_back(e);
                    events->pop();
                }

                if (isMultiCore) {
                    if (loadBalance) {
                        schedAlgo.getMaximumVtimeDiff(multicoreFairness);

                        schedAlgo.loadBalance(osTime);

                        loadBalance = false;
                    }

                    Metrics aux("none");
                    for (const auto &e:currentEvents) {
                        if (e.getType() == LOADBALANCE) {

                            loadBalance = true;
                            for (int i=0; i<numCPUS; i++) {
                                core[i]->addEvent(Event(LOADBALANCE, currentTime, Process()));
                            }
                            timerExpired = true;
                        }
//                        if (e.getType() == IDLE) {
//                            cout << "aici\n";
//                        }
                    }
                    if (osTime >= 1000000) {
                        timerExpired = false;
                    }
                    if (!allFinished) {
                        schedAlgo.schedule(currentTime, aux, timerExpired);
                    }

                    for (const auto &event : currentEvents) {
                        if (event.getType() == ARRIVAL) {
                            core[schedAlgo.assignCPU(event.getProcess())]->addEvent(event);
                        }
                    }
//                    if (currentTime > 0) {
//                        events->push(Event(IDLE, currentTime, Process()));
//                    }
                    osTime = currentTime;
                }
                else if (isRealTime()) {
                    vector<Process> arrivals;
                    for (const auto &event : currentEvents) {
                        if (event.getType() == ARRIVAL) {
                            Process p(event.getProcess());
                            p.setPriority(p.getRemainingBurst());
                            p.setNextDeadline(currentTime + p.getPeriod());
                            arrivals.push_back(p);
                            events->push(Event(ARRIVAL, p.getPeriod() + currentTime, p));
                        } else if (event.getType() == REALTIME){
                            arrivals.push_back(event.getProcess());
                        }
                    }
                    schedAlgo.processArrived(arrivals, currentTime, realTimeMetrics);
                    schedAlgo.schedule(currentTime, realTimeMetrics, false);
                    osTime = currentTime;
                } else {
                    for (const auto &event : currentEvents) {
                        if (event.getType() == ARRIVAL) {
                            core[schedAlgo.assignCPU(event.getProcess())]->addEvent(event);
                        }
                    }
                    osTime = currentTime;
                }
            }
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
        secondBarrier.arrive_and_wait();

    }
    vector<Metrics> vec;

    for (int i=0; i<numCPUS; i++) {
        vec.push_back(core[i]->join());
    }

    if (realTime) {
        vec.push_back(realTimeMetrics);
    }

    ofstream out("output.txt");
    for (auto v:multicoreFairness.getMaximumLoadDifference()) {
        out << v << " ";
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

void DES::setInputFromString(const string &input, bool realTime) {
    stringstream ss(input);
    for (auto x:generatedBurstByDistributionId) {
        generatedBurstByDistributionId[x.first].clear();
    }
    events = new priority_queue<Event>();
    if (realTime) {
        vector<int> periods;
        string line;
        while (getline(ss, line)) {
            if (!line.empty()) {
                Event event = Event::fromString(line);
                events->push(event);
                periods.push_back(event.getProcess().getPeriod());
            }
        }
        int hyperPeriod = std::lcm(periods[0], periods[1]);
        for (int i=2; i<periods.size(); i++) {
            hyperPeriod = std::lcm(hyperPeriod, periods[i]);
        }

        setToStop(hyperPeriod);
    } else {
        string firstLine, secondLine, thirdLine;
        vector<Event> eventVec;
        for (int i=0; i<3; i++) {
            generatedBurstByDistributionMean[i] = 0;
            generatedBurstByDistributionSTD[i] = 0;
        }
        while (getline(ss, firstLine) && getline(ss, secondLine) && getline(ss, thirdLine)) {
            if (!firstLine.empty()) {
                numberOfProcesses += 1;
                Event event = Event::fromStrings(firstLine, secondLine, thirdLine);
                events->push(event);
                generatedBurstByDistributionId[event.getProcess().getDistributionId()].push_back(event.getProcess().getRemainingBurst());
            }
        }
    }

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

string DES::generateInputData(int numberOfTasks, int numberOfCores) {
    std::vector<std::pair<int,int>> tasks = RealTimeGenerator::generateTasks(numberOfTasks, numberOfCores);
    string inputData;
    events = new priority_queue<Event>();

    for (auto task:tasks) {
        Process p(task.first, task.second);
        inputData += to_string(p.getId()) + " " + to_string(task.first) + " " + to_string(task.second) + "\n";
        p.setArrivalTime(0);

        Event event = Event(ARRIVAL, 0, p);

        events->push(event);
    }

    int hyperPeriod = std::lcm(tasks[0].second, tasks[1].second);
    for (int i=2; i<tasks.size(); i++) {
        hyperPeriod = std::lcm(hyperPeriod, tasks[i].second);
    }

    setToStop(hyperPeriod);

    return inputData;
}

bool DES::isRealTime() const {
    return realTime;
}

void DES::setRealTime(bool realTime) {
    DES::realTime = realTime;
}

int DES::isToStop() const {
    return toStop;
}

void DES::setToStop(int toStop) {
    DES::toStop = toStop;
}

void DES::setIsMultiCore(bool isMultiCore) {
    DES::isMultiCore = isMultiCore;
}

Metrics DES::evaluatePerformance() {
//    return Metrics(__cxx11::basic_string());
}

vector<vector<pair<int, int>>> DES::generateTaskSet(int perTaskNum, double utilization) {
    return RealTimeGenerator::generateTaskSet(perTaskNum, utilization);
}

void DES::setEvents(priority_queue<Event> *events) {
    DES::events = events;
}

const map<int, vector<int>> &DES::getGeneratedBurstByDistributionId() const {
    return generatedBurstByDistributionId;
}

void DES::setGeneratedBurstByDistributionId(const map<int, vector<int>> &generatedBurstByDistributionId) {
    DES::generatedBurstByDistributionId = generatedBurstByDistributionId;
}

map<int, int> &DES::getGeneratedBurstByDistributionMean() {
    return generatedBurstByDistributionMean;
}

void DES::setGeneratedBurstByDistributionMean(const map<int, int> &generatedBurstByDistributionMean) {
    DES::generatedBurstByDistributionMean = generatedBurstByDistributionMean;
}

map<int, int> &DES::getGeneratedBurstByDistributionStd() {
    return generatedBurstByDistributionSTD;
}

void DES::setGeneratedBurstByDistributionStd(const map<int, int> &generatedBurstByDistributionStd) {
    generatedBurstByDistributionSTD = generatedBurstByDistributionStd;
}
