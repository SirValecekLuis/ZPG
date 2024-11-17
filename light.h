#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>

enum class LightType {
    POINT,
    DIRECTIONAL,
    SPOTLIGHT
};

class Light {
public:
    explicit Light(const LightType type,
          const glm::vec3 &position = glm::vec3(0.0f, 1.0f, 0.0f),
          const glm::vec3 &color = glm::vec3(1.0f),
          const float intensity = 1.0f)
        : type(type), position(position), color(color), intensity(intensity) {
        if (type == LightType::DIRECTIONAL) {
            direction = glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f));
        } else {
            direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
        }
        cutOff = glm::cos(glm::radians(12.5f));
        outerCutOff = glm::cos(glm::radians(17.5f));
    }

    LightType type;
    glm::vec3 position;
    glm::vec3 direction{};
    glm::vec3 color;
    float intensity;
    float cutOff;
    float outerCutOff;
};

#endif // LIGHT_H
