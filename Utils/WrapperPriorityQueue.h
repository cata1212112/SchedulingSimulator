//
// Created by Cata on 2/29/2024.
//

#ifndef LICENTA_WRAPPERPRIORITYQUEUE_H
#define LICENTA_WRAPPERPRIORITYQUEUE_H

#include <queue>
#include <map>
#include <set>

template<typename Compare = std::less<Process>>
class WrapperPriorityQueue {
private:
    std::multiset<Process, Compare> pq;
public:

    WrapperPriorityQueue(Compare comp) : pq(comp) {}

    void push(const Process& value) {
        pq.insert(value);
    }

    void pop() {
        pq.erase(pq.begin());
    }

    const Process& top() const {
        return *pq.begin();
    }

    bool empty() const { return pq.empty(); }

    int size() const { return pq.size(); }

    int getMaximumElement() {
        return pq.end()->getRemainingBurst();
    }

    vector<Process> getAllProcesses() {
        vector<Process> procs;
        for (auto it=pq.begin(); it != pq.end(); it++) {
            procs.push_back(Process(*it));
        }
        return procs;
    }
};


#endif //LICENTA_WRAPPERPRIORITYQUEUE_H
