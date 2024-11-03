//
// Created by tobiasjanca on 10/20/24.
//

#ifndef CAMERA_H
#define CAMERA_H


#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <GL/glew.h>

#include "observer.h"


enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera final : public Subject {
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f),
                    float pitch = -90.0f,
                    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                    float yaw = -90.0f);


    [[nodiscard]] glm::mat4 get_view_matrix() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;

    void move(const glm::vec3 &offset);

    void rotate(float yaw, float pitch);

    void add_observer(Observer *observer) override;

    void remove_observer(Observer *observer) override;

    void notify_observers() override;

    void process_keyboard(CameraMovement direction, float delta_time);

    void process_mouse_movement(float offset_x, float offset_y, GLboolean constrain_pitch = true);

    void update_camera_vectors();

    [[nodiscard]] glm::vec3 get_camera_pos() const;

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;
    float zoom;

    std::vector<Observer *> observers;
};

#endif //CAMERA_H
