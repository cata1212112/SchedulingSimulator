//
// Created by Cata on 2/12/2024.
//

#include <sstream>
#include "Event.h"

string Event::toString() {
    string eventString;
    eventString += to_string(time) + " " + to_string(process.getId()) + " " + to_string(process.getPriority()) + " " + to_string(process.getDistributionId()) +"\n";

    for (auto cpuBurst : process.getCpUburst()) {
        eventString += to_string(cpuBurst) + " ";
    }

    eventString += "\n";

    for (auto ioBurst : process.getIOburst()) {
        eventString += to_string(ioBurst) + " ";
    }

    eventString += "\n";

    return eventString;
}

Event Event::fromStrings(const string& firstLine, const string &secondLine, const string &thirdLine) {
    Event event;

    stringstream firstSS(firstLine);
    firstSS >> event.time;

    int processID;

    firstSS >> processID;

    event.process.setId(processID);

    int processPriority;

    firstSS >> processPriority;
    event.process.setPriority(processPriority);

    int distribtuionId;
    firstSS >> distribtuionId;
    event.process.setDistributionId(distribtuionId);

    vector<int> cpuBursts;
    vector<int> ioBursts;


    int cpuB;
    stringstream CPUBURSTS(secondLine);

    while (CPUBURSTS >> cpuB) {
        cpuBursts.push_back(cpuB);
    }

    int ioB;
    stringstream IOBURSTS(thirdLine);

    while (IOBURSTS >> ioB) {
        ioBursts.push_back(ioB);
    }

    event.type = ARRIVAL;

    event.process.setCpUburst(cpuBursts);
    event.process.setIOburst(ioBursts);

    event.process.setRemainingBurst(cpuBursts[0]);

    event.process.setArrivalTime(event.time);

    return event;
}

Event Event::fromString(const string &line) {
    Event event;

    stringstream ss(line);

    int processID;

    ss >> processID;

    event.process.setId(processID);

    int burst;

    ss >> burst;
    event.process.setCpUburst({burst});

    int deadline;
    ss >> deadline;
    event.process.setAbsoluteDeadline(deadline);
    event.process.setPeriod(deadline);
    event.process.setArrivalTime(0);
    event.type = ARRIVAL;
    event.process.setRemainingBurst(burst);
    event.time = 0;
    return event;
}


Event::Event() {}

eventType Event::getType() const {
    return type;
}

int Event::getTime() const {
    return time;
}

const Process &Event::getProcess() const {
    return process;
}

Event::~Event() {

}
