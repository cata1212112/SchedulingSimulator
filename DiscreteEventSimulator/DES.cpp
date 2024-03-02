//
// Created by Cata on 2/12/2024.
//

#include "DES.h"
#include <map>

string DES::generateInputData(int numProcesses, int maximumTime=MAXIMUMARRIVAL) {

    events = new priority_queue<Event>();

    numberOfProcesses = numProcesses;

    string inputData;
    for (int i=0; i<numProcesses; i++) {
        int arrival = Random::randomInteger(maximumTime);
        int numBursts = Random::randomInteger(MAXIMUMNUMCPUBURSTS) + 1;
        bool processType = Random::randomBit();
        int priority = Random::randomInteger(10);

        vector<int> cpuBursts;
        vector<int> ioBursts;

        for (int j=0; j<numBursts; j++) {
            int cpuBurst = Random::randomBurst(getMultiplier(processType));
            int ioBurst = Random::randomBurst(getMultiplier(!processType));

            cpuBursts.push_back(cpuBurst);
            ioBursts.push_back(ioBurst);
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

Metrics DES::startSimulation(int numCPUS) {
    SchedulingAlgorithm &schedAlgo = ImplementedAlgorithms::getAlgorithm(algorithm, roundRobinQuant);

    Metrics stats(algorithm);
    int currentTime = 0;

    while (!events->empty()) {
        Event e = events->top();
        events->pop();

        currentTime = e.getTime();

        vector<Event> currentEvents;
        currentEvents.push_back(e);

        while (!events->empty() && events->top().getTime() == currentTime) {
            e = events->top();
            currentEvents.push_back(e);
            events->pop();
        }

        vector<Event> arrived;
        vector<Event> cpuComplete;
        vector<Event> ioComplete;
        vector<Event> preemnt;
        vector<Event> timerExpired;

        copy_if(currentEvents.begin(), currentEvents.end(), back_inserter(arrived), [](const Event& a) {return a.getType() == ARRIVAL;});
        copy_if(currentEvents.begin(), currentEvents.end(), back_inserter(cpuComplete), [](const Event& a) {return a.getType() == CPUBURSTCOMPLETE;});
        copy_if(currentEvents.begin(), currentEvents.end(), back_inserter(ioComplete), [](const Event& a) {return a.getType() == IOBURSTCOMPLETE;});
        copy_if(currentEvents.begin(), currentEvents.end(), back_inserter(preemnt), [](const Event& a) {return a.getType() == PREEMT;});
        copy_if(currentEvents.begin(), currentEvents.end(), back_inserter(timerExpired), [](const Event& a) {return a.getType() == TIMEREXPIRED;});



        if (!preemnt.empty()) {
            priority_queue<Event> tmp;
            map<int, bool> toPreempt;
            for (const auto &event : preemnt) {
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

        for (const auto& event : arrived) {
            arrivedProcesses.push_back(event.getProcess());
        }

        for (const auto &event : ioComplete) {
            ioCompleteProcesses.push_back(event.getProcess());
        }
        vector<Event> eventsGenerated;

        schedAlgo.processArrived(arrivedProcesses, currentTime, stats);
        schedAlgo.processIOComplete(ioCompleteProcesses, currentTime, stats);
        if (!cpuComplete.empty()) {
            eventsGenerated = schedAlgo.processCPUComplete(cpuComplete[0].getProcess(), currentTime, stats);
        }

        vector<Event> aux = schedAlgo.schedule(currentTime, stats, !timerExpired.empty());
        eventsGenerated.insert(eventsGenerated.end(), aux.begin(), aux.end());

        for (const auto& eventGenerated : eventsGenerated) {
            addEventToQueue(eventGenerated);
        }
    }
    stats.divide(currentTime, numberOfProcesses);
    return stats;
}

bool DES::isUsedFileAsInput() const {
    return usedFileAsInput;
}

const string &DES::getAlgorithm() const {
    return algorithm;
}

const string &DES::getInput() const {
    return input;
}

void DES::setInput(const string &input) {
    DES::input = input;
}

void DES::setAlgorithm(const string &algorithm) {
    DES::algorithm = algorithm;
}

void DES::setInputFromString(const string &input) {
    stringstream ss(input);
    events = new priority_queue<Event>();

    string firstLine, secondLine, thirdLine;
    while (getline(ss, firstLine) && getline(ss, secondLine) && getline(ss, thirdLine)) {
        if (!firstLine.empty()) {
            numberOfProcesses += 1;
            Event event = Event::fromStrings(firstLine, secondLine, thirdLine);
            events->push(event);
        }
    }
}

const string &DES::getPartialMetricsInput() const {
    return partialMetricsInput;
}

void DES::setPartialMetricsInput(const string &partialMetricsInput) {
    DES::partialMetricsInput = partialMetricsInput;
}

void DES::addEventToQueue(Event e) {
    events->push(e);
}

int DES::getRoundRobinQuant() const {
    return roundRobinQuant;
}

void DES::setRoundRobinQuant(int roundRobinQuant) {
    DES::roundRobinQuant = roundRobinQuant;
}
