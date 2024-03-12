//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_DES_H
#define CPUSCHED_DES_H

#include <string>
#include "../Utils/Random.h"
#include "../Utils/ImplementedAlgorithms.h"
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>

#define MAXIMUMNUMCPUBURSTS 20
#define MAXIMUMARRIVAL 300

using namespace std;

class DES {
private:
    priority_queue<Event> *events;
    string input;
    string algorithm;
    bool usedFileAsInput = false;
    int numberOfProcesses = 0;
    string partialMetricsInput = "";
    vector<pair<string, string>> partialGantt;
    int roundRobinQuant;
    std::condition_variable cv;
    std::mutex cvMutex;
    bool continueExecution = true;
    bool osTimeUpdated = false;

    static int getMultiplier(bool flag) {
        if (flag) {
            return 10;
        } else {
            return 5;
        }
    }

public:
    int getRoundRobinQuant() const;

    void setRoundRobinQuant(int roundRobinQuant);


    DES(string algorithm) : algorithm(std::move(algorithm)) {
        events = new priority_queue<Event>();
    }

    string generateInputData(int numProcesses, int maximumTime);

    void readInputDataFromFile(const string& filename);

    void setInputFromString(const string& input);

    Metrics startSimulation(int numCPUS);

    bool isUsedFileAsInput() const;

    void setAlgorithm(const string &algorithm);

    const string &getAlgorithm() const;

    const string &getInput() const;

    void setInput(const string &input);

    const string &getPartialMetricsInput() const;

    void setPartialMetricsInput(const string &partialMetricsInput);

    void addToGantt(const pair<string, string> &gantt) {
        partialGantt.push_back(gantt);
    }

    vector<pair<string, string>> getGantt() {
        return partialGantt;
    }

    void addEventToQueue(Event e);
};


#endif //CPUSCHED_DES_H
