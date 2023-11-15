#pragma once
#include "Essentials.hpp"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace db3d {
    class Camera
    {
    public:
        explicit Camera(f32 verticalFOV, f32 nearClip, f32 farClip);
        bool OnUpdate(f32 dt);
        void OnResize(u64 width, u64 height);

        template<typename T = f32>
        std::array<T,2> GetViewportSize() const { return { T(m_viewportWidth), T(m_viewportHeight) }; }

        const glm::mat4& GetProjection() const { return m_projection; }
        const glm::mat4& GetInverseProjection() const { return m_inverseProjection; }
        const glm::mat4& GetView() const { return m_view; }
        const glm::mat4& GetInverseView() const { return m_inverseView; }
        const glm::vec3& GetPosition() const { return m_position; }
        const glm::vec3& GetForwardDirection() const { return m_forwardDirection; }
        const glm::quat& GetRotationQuat() const { return m_rotation; }
        f32 GetSpeed() const { return m_moveSpeed; }
        const std::vector<glm::vec3>& GetRayDirections() const { return m_rayDirections; }

        void SetPosition(glm::vec3 position) { m_position = position; }

        f32 GetRotationSpeed() const { return m_rotationSpeed; }

        glm::vec3 ScreenToWorld(const glm::vec2& screenCoord) const;

        glm::vec2 WorldToScreen(const glm::vec3& worldCoord) const;

    private:
        void RecalculateProjection();
        void RecalculateView();
        void RecalculateRayDirections();

    private:
        f32 m_verticalFOV = 45.0f;
        f32 m_nearClip = 0.1f;
        f32 m_farClip = 100.0f;

        glm::mat4 m_projection{ 1.0f };
        glm::mat4 m_view{ 1.0f };
        glm::mat4 m_inverseProjection{ 1.0f };
        glm::mat4 m_inverseView{ 1.0f };

        glm::vec3 m_position{ 0.0f, 0.0f, 10.0f };
        glm::vec3 m_forwardDirection{ 0.0f, 0.0f, -1.0f };
        glm::quat m_rotation{ 1.0f, 0.0f, 0.0f, 0.0f };

        // Cached ray directions
        std::vector<glm::vec3> m_rayDirections;

        glm::vec2 m_lastMousePosition{ 0.0f, 0.0f };

        u64 m_viewportWidth = 0;
        u64 m_viewportHeight = 0;

        f32 m_moveSpeed = 2.0f;
        f32 m_moveSpeedDelta = 0.2f;
        f32 m_moveSpeedMax = 30.0f;
        f32 m_rotationSpeed = 0.15f;
    };
}
