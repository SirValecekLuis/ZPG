#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "light.h"
#include "camera.h"

class FlashLight final : public Light, public Observer {
public:
    FlashLight() : Light(LightType::SPOTLIGHT) {
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        intensity = 1.5f;
        cutOff = glm::cos(glm::radians(17.5f));
        outerCutOff = glm::cos(glm::radians(22.5f));
    }

    void update(Subject *subject) override {
        if (const auto *camera = dynamic_cast<Camera *>(subject)) {
            position = camera->get_camera_pos();
            direction = camera->get_front();
        }
    }
};

#endif // FLASHLIGHT_H