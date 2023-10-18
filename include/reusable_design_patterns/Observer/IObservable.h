#pragma once

#include "vector"
#include "IObserver.h"

class IObservable {
public:
    virtual void subscribe(IObserver *observer) = 0;

    virtual void unsubscribe(IObserver *observer) = 0;

private:
    virtual void notify(int event_type) = 0;
};