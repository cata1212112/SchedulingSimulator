//
// Created by Cata on 2/12/2024.
//

#include "DES.h"

string DES::generateInputData(int numProcesses, int maximumTime=MAXIMUMARRIVAL) {

    events = new priority_queue<Event>();

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

    string firstLine, secondLine, thirdLine;
    while (getline(in, firstLine) && getline(in, secondLine) && getline(in, thirdLine)) {
        Event event = Event::fromStrings(firstLine, secondLine, thirdLine);
        events->push(event);
    }

    in.close();
}

Metrics DES::startSimulation(int numCPUS) {
    SchedulingAlgorithm &schedAlgo = ImplementedAlgorithms::getAlgorithm(algorithm);
    Metrics stats(algorithm);
    int currentTime = 0;
    while (!events->empty()) {
        Event e = events->top();
        events->pop();

        currentTime = e.getTime();

        vector<Event> eventsGenerated;

        switch (e.getType()) {
            case ARRIVAL:
                eventsGenerated = schedAlgo.processArrived(e.getProcess(), currentTime, stats);
                break;
            case CPUBURSTCOMPLETE:
                eventsGenerated = schedAlgo.processCPUComplete(e.getProcess(), currentTime, stats);
                break;
            case IOBURSTCOMPLETE:
                eventsGenerated = schedAlgo.processIOComplete(e.getProcess(), currentTime, stats);
                break;
            case PREEMT:
                eventsGenerated = schedAlgo.processPreempt(e.getProcess(), currentTime, stats);
                break;
            case TIMEREXPIRED:
                break;
        }

        for (const auto& eventGenerated : eventsGenerated) {
            events->push(eventGenerated);
        }
    }
    return stats;
}

bool DES::isUsedFileAsInput() const {
    return usedFileAsInput;
}
