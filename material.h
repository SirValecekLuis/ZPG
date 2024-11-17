#ifndef MATERIAL_H
#define MATERIAL_H

#include <random>

class Material {
public:
    explicit Material(const float ra = 0.1f, const float rd = 0.5f, const float rs = 0.4f)
        : ra(ra), rd(rd), rs(rs), randomEngine(std::random_device{}()) {}

    void set_coefficients(const float ra, const float rd, const float rs) {
        this->ra = ra;
        this->rd = rd;
        this->rs = rs;
    }

    void randomize() {
        std::uniform_real_distribution<float> ambientDist(0.1f, 0.1f);
        std::uniform_real_distribution<float> diffuseDist(0.5f, 1.0f);
        std::uniform_real_distribution<float> specularDist(0.0f, 1.0f);

        ra = ambientDist(randomEngine);
        rd = diffuseDist(randomEngine);
        rs = specularDist(randomEngine);
    }

    float get_ra() const { return ra; }
    float get_rd() const { return rd; }
    float get_rs() const { return rs; }

private:
    float ra;
    float rd;
    float rs;

    std::mt19937 randomEngine;
};

#endif // MATERIAL_H
