#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <xengine/rendering/material.hpp>

namespace xe_def {
    struct LitMaterial : public XEngine::Material {
        glm::vec4 ambient = glm::vec4(0);
        glm::vec4 diffuse = glm::vec4(1);
        glm::vec4 specular = glm::vec4(0);
        glm::vec4 emission = glm::vec4(0);
        glm::vec4 emission_color = glm::vec4(0);
        float emission_factor = 0.f;
        float shininess = 0.5f;
    };
    LitMaterial model_mat = { XEngine::Shader(), glm::vec4(0), glm::vec4(1), glm::vec4(0), glm::vec4(0), glm::vec4(0), 0.f, 0.5f };
}
#endif // DEFAULTS_H