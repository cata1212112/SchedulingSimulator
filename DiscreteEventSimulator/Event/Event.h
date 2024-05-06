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
    TIMEREXPIRED,
    PREEMT,
    FINISHEXECUTION,
    REALTIME,
    TICK,
    LOADBALANCE,
    THROTTLE,
    NUM_EVENTS
};

class Event {
private:
    eventType type;
    int time;
    Process process;
public:
    eventType getType() const;

    int getTime() const;

    const Process &getProcess() const;

    Event();

    bool operator<(const Event &other) const {
        if (time == other.time) {
            return process.getId() > other.process.getId();
        }
        return time > other.time;
    }

    Event(eventType type, int time, const Process& process) : type(type), time(time), process(process) {}

    string toString();

    static Event fromStrings(const string &firstLine, const string &secondLine, const string &thirdLine);

    static Event fromString(const string &line);

    virtual ~Event();
};


#endif //CPUSCHED_EVENT_H
