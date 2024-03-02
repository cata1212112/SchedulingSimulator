//
// Created by Cata on 2/29/2024.
//

#ifndef LICENTA_METRICS_H
#define LICENTA_METRICS_H

#include <vector>

class Metrics {
private:
    double cpuUtilization = 0;
    double averageWaitingTime = 0;
    double averageTurnaroundTime = 0;
    double averageResponseTime = 0;
    std::string algorithm;
    std::vector<std::tuple<int, int, int>> gantt;
public:
    explicit Metrics(const std::string &algorithm) : algorithm(algorithm) {}


    void addToGanttChart(int pid, int left, int right) {
        gantt.push_back({pid, left, right});
    }

    std::string getGanttData() {
        std::string data = "[";
        for (auto x : gantt) {
            data += "[" + std::to_string(std::get<0>(x)) + "," + std::to_string(std::get<1>(x)) + "," + std::to_string(std::get<2>(x)) + "],";
        }
        data.pop_back();
        data += "]";
        return data;
    }

    void addToCPUUtilization(int val) {
        cpuUtilization += val;
    }

    void addToWT(int val) {
        averageWaitingTime += val;
    }

    void addToTT(int val) {
        averageTurnaroundTime += val;
    }

    void addToRT(int val) {
        averageResponseTime += val;
    }

    std::string getMetrics() {
        return "\"" + algorithm + "\" " + std::to_string(cpuUtilization) + " " + std::to_string(averageWaitingTime) + " " + std::to_string(averageTurnaroundTime) + " " +
                std::to_string(averageResponseTime);
    }

    void divide(double timespan, double numOfProcs) {
        cpuUtilization = cpuUtilization / timespan * 100;
        averageWaitingTime /= numOfProcs;
        averageResponseTime /= numOfProcs;
        averageTurnaroundTime /= numOfProcs;
    }
};


#endif //LICENTA_METRICS_H
