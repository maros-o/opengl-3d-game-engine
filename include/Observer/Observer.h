#pragma once

class Observer {
public:
    virtual void update(int event) = 0;
};