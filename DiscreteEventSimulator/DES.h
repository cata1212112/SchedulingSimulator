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
    string partialMetricsInput[32];
    vector<pair<string, string>> partialGantt[32];
    int roundRobinQuant;
    std::condition_variable cv;
    std::mutex cvMutex;
    bool osTimeUpdated = false;
    bool osTurn = true;
    static vector<string> algortihms;
    bool realTime = false;
    bool isMultiCore = false;
    int toStop = 0;
public:
    int isToStop() const;

    void setToStop(int toStop);

public:
    bool isRealTime() const;

    void setRealTime(bool realTime);

private:

    static double getMultiplier(bool flag) {
        if (flag) {
            return 1;
        } else {
            return 1;
        }
    }

public:
    int getRoundRobinQuant() const;

    void setRoundRobinQuant(int roundRobinQuant);


    DES(string algorithm) : algorithm(algorithm) {
        algortihms.push_back(algorithm);
        events = new priority_queue<Event>();
        roundRobinQuant = 10;
    }

    string generateInputData(int numProcesses, int maximumTime, int mean, int std);

    string generateInputData(int numberOfTasks, int numberOfCores);

    void readInputDataFromFile(const string& filename, bool realTime = false);

    void setInputFromString(const string& input, bool realTime = false);

    vector<Metrics> startSimulation(int numCPUS);

    bool isUsedFileAsInput() const;

    void setAlgorithm(const string &algorithm);

    string getAlgorithm() const;

    const string &getInput() const;

    void setInput(const string &input);

    const string &getPartialMetricsInput(int core) const;

    void setEvents(priority_queue<Event> *events);

    void setPartialMetricsInput(const string &partialMetricsInput, int core);

    void addToGantt(const pair<string, string> &gantt, int core) {
        partialGantt[core].push_back(gantt);
    }

    vector<pair<string, string>> getGantt(int core) {
        return partialGantt[core];
    }

    static vector<string> getAlgorithms() {
        return algortihms;
    }

    void setIsMultiCore(bool isMultiCore);

    Metrics evaluatePerformance();

    static vector<vector<pair<int,int>>> generateTaskSet(int perTaskNum, double utilization);
};


#endif //CPUSCHED_DES_H
