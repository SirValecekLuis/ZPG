//
// Created by tobiasjanca on 10/20/24.
//

#include "camera.h"

#include <bits/stl_algo.h>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), world_up(up), yaw(yaw),
      pitch(pitch), movement_speed(2.5f),
      mouse_sensitivity(0.1f), zoom(45.0f) {
    update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(glm::radians(zoom), 16.0f / 9.0f, 0.1f, 100.0f);
}

void Camera::move(const glm::vec3 &offset) {
    position += offset;
    notify_observers();
}

void Camera::rotate(float yaw, float pitch) {
    this->yaw += yaw * mouse_sensitivity;
    this->pitch += pitch * mouse_sensitivity;

    if (this->pitch > 89.0f) this->pitch = 89.0f;
    if (this->pitch < -89.0f) this->pitch = -89.0f;

    update_camera_vectors();
    notify_observers();
}

void Camera::update_camera_vectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::add_observer(Observer *observer) {
    observers.push_back(observer);
}

void Camera::remove_observer(Observer *observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Camera::notify_observers() {
    for (const auto observer: observers) {
        observer->update(this);
    }
}

void Camera::process_keyboard(const CameraMovement direction, const float deltaTime)
{
    float velocity = movement_speed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;

    notify_observers();
}

void Camera::process_mouse_movement(float xoffset, float yoffset, const GLboolean constrainPitch)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    update_camera_vectors();
    notify_observers();
}
