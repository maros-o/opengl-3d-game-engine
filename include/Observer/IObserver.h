#pragma once

class IObserver {
public:
    virtual void update(int event) = 0;
};