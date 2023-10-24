#pragma once

#include <algorithm>
#include "vector"
#include "Observer.h"

class Observable {
public:
    virtual void subscribe(Observer *observer) {
        observers.push_back(observer);
    }

    virtual void unsubscribe(Observer *observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

protected:
    virtual void notify(int event) {
        for (auto &observer: observers) {
            observer->update(event);
        }
    }

    std::vector<Observer *> observers;
};