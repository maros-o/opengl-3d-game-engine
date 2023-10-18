#pragma once

class IObserver {
public:
    virtual void update(int event_type) = 0;
};