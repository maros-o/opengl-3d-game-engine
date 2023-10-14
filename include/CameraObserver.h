#pragma once

class CameraObserver {
public:
    virtual void view_matrix_changed() = 0;

    virtual void projection_matrix_changed() = 0;
};