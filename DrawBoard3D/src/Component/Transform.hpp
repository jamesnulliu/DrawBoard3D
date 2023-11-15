#pragma once
#include "Component.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace db3d {
    class Transform : public Component
    {
    public:
        glm::vec3 translate = { 0,0,0 };
        glm::quat rotate = { 1,0,0,0 };
        glm::vec3 scale = { 1,1,1 };
    };
}
