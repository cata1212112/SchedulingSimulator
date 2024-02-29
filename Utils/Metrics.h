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
    std::vector<std::tuple<int, int, int>> gantt;
public:
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
};


#endif //LICENTA_METRICS_H
