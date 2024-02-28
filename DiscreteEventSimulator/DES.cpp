//
// Created by Cata on 2/12/2024.
//

#include "DES.h"

string DES::generateInputData(int numProcesses, int maximumTime=MAXIMUMARRIVAL) {

    events = new priority_queue<Event>();

    string inputData;
    for (int i=0; i<numProcesses; i++) {
        int arrival = Random::randomInteger(maximumTime);
        int numBursts = Random::randomInteger(MAXIMUMNUMCPUBURSTS);
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

        Event event = Event(ARRIVAL, arrival, p);

        inputData += event.toString();

        events->push(event);
    }

    return inputData;
}

void DES::readInputDataFromFile(const string& filename) {
    events = new priority_queue<Event>();

    ifstream in(filename);

    string firstLine, secondLine, thirdLine;
    while (getline(in, firstLine) && getline(in, secondLine) && getline(in, thirdLine)) {
        Event event = Event::fromStrings(firstLine, secondLine, thirdLine);
        events->push(event);
    }

    in.close();
}

void DES::startSimulation(SchedulingAlgorithm &schedAlgo, int numCPUS) {
//    char* json = generateInputData(numCPUS);
}