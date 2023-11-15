#pragma once
#include "Component/Transform.hpp"
#include "Component/Material.hpp"
#include "Component/VertexVec.hpp"

namespace db3d {
    class Entity
    {
    public:
        virtual ~Entity() = default;
        virtual void Draw() = 0;

    private:
        Transform trsf;
        VertexVec vertices;
    };
}
