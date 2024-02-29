//
// Created by Cata on 2/29/2024.
//

#ifndef LICENTA_WRAPPERPRIORITYQUEUE_H
#define LICENTA_WRAPPERPRIORITYQUEUE_H

#include <queue>


template<typename T, typename Compare = std::less<T>>
class WrapperPriorityQueue {
private:
    std::priority_queue<T, std::vector<T>, Compare> pq;
public:

    WrapperPriorityQueue(Compare comp) : pq(comp) {}

    void push(const T& value) {pq.push(value);}

    void pop() {pq.pop();}

    const T& top() const { return pq.top(); }

    bool empty() const { return pq.empty(); }

    int size() const { return pq.size(); }
};


#endif //LICENTA_WRAPPERPRIORITYQUEUE_H
