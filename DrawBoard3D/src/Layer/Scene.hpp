#pragma once
#include "Essentials.hpp"
#include <memory>
#include <vector>

#include "Camera/Camera.hpp"
#include "Entity/Entity.hpp"

namespace db3d {
    class Scene 
    {
    public:
        std::vector<int> arr;
        std::shared_ptr<Camera> activeCamera = nullptr;
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<Entity>> entities;
    };
}
