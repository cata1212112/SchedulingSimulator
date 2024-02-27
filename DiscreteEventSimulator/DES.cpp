//
// Created by Cata on 2/12/2024.
//

#include "DES.h"

char* DES::generateInputData(int numProcesses, int maximumTime=MAXIMUMARRIVAL) {
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

        events.push(event);
    }
}

unordered_map<string, vector<Event>> DES::readInputDataFromFile(const string& filename) {
    ifstream in(filename);
    ostringstream ss;
    ss << in.rdbuf();
    const string inputJSON = ss.str();
    return readInputDataFromJSON(inputJSON);
}

unordered_map<string, vector<Event>> DES::readInputDataFromJSON(const string& json) {
    unordered_map<string, vector<Event>> result;
    return result;
}

void DES::startSimulation(SchedulingAlgorithm &schedAlgo, int numCPUS) {
    char* json = generateInputData(numCPUS);
}
