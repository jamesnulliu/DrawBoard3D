#pragma once
#include "Essentials.hpp"
#include "Component.hpp"

#include "glm/glm.hpp"

namespace db3d {
    class Material : public Component
    {
    public:
        glm::vec4 color;
        f32 roughness;
    };
}
