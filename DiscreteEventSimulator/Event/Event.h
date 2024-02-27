//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_EVENT_H
#define CPUSCHED_EVENT_H

#include "../../Process/Process.h"
#include <memory>
#include <string>

using namespace std;

enum eventType {
    ARRIVAL,
    CPUBURSTCOMPLETE,
    IOBURSTCOMPLETE,
    TIMEREXPIRED
};

class Event {
private:
    eventType type;
    int time;
    Process process;
public:
    bool operator<(const Event &other) const {
        return time < other.time;
    }

    Event(eventType type, int time, const Process& process) : type(type), time(time), process(process) {}
};


#endif //CPUSCHED_EVENT_H
