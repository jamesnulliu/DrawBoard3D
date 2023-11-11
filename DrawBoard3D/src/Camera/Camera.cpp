#include "Camera.hpp"
#include "Walnut/Input/Input.h"

namespace db3d {
    Camera::Camera(f32 verticalFOV, f32 nearClip, f32 farClip)
        : m_verticalFOV(verticalFOV), m_nearClip(nearClip), m_farClip(farClip)
    {
        m_rotation = glm::rotation({ 0.0, 0.0, -1.0 }, m_forwardDirection);
    }

    bool Camera::OnUpdate(f32 dt)
    {
        glm::vec2 mousePos = Walnut::Input::GetMousePosition();
        glm::vec2 delta = (mousePos - m_lastMousePosition) * 0.003f;
        m_lastMousePosition = mousePos;

        if (!Walnut::Input::IsMouseButtonDown(Walnut::MouseButton::Right))
        {
            Walnut::Input::SetCursorMode(Walnut::CursorMode::Normal);
            return false;
        }

        Walnut::Input::SetCursorMode(Walnut::CursorMode::Locked);

        bool moved = false;

        constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
        glm::vec3 rightDirection = glm::cross(m_forwardDirection, upDirection);


        // Movement
        if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W)) {
            m_position += m_forwardDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S)) {
            m_position -= m_forwardDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        }else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A)) {
            m_position -= rightDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D)) {
            m_position += rightDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::Q)) {
            m_position -= upDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::E)) {
            m_position += upDirection * m_speed * dt;
            moved = true;
            if (m_speed <= m_maxSpeed) m_speed += m_deltaSpeed;
        } else {
            m_speed = 2.0f;
        }

        // Rotation
        if (delta.x != 0.0f || delta.y != 0.0f) {
            f32 pitchDelta = delta.y * GetRotationSpeed();
            f32 yawDelta = delta.x * GetRotationSpeed();

            glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
                glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));
            m_forwardDirection = glm::rotate(q, m_forwardDirection);
            m_rotation = glm::rotation({ 0.0, 0.0, -1.0 }, m_forwardDirection);

            moved = true;
        }

        if (moved) {
            RecalculateView();
            RecalculateRayDirections();
        }

        return moved;
    }

    void Camera::OnResize(u32 width, u32 height)
    {
        if (width == m_viewportWidth && height == m_viewportHeight)
            return;

        m_viewportWidth = width;
        m_viewportHeight = height;

        RecalculateProjection();
        RecalculateView();
        RecalculateRayDirections();
    }

    f32 Camera::GetRotationSpeed()
    {
        return 0.15f;
    }

    glm::vec2 Camera::WorldToScreen(const glm::vec3& point) const
    {
        glm::vec4 cameraSpacePoint = m_view * glm::vec4(point, 1.0f);
        glm::vec4 clipSpacePoint = m_projection * cameraSpacePoint;
        if (clipSpacePoint.w <= 0.0F) { return glm::vec2(-1.0f, -1.0f); }
        glm::vec2 screenSpacePoint{};
        screenSpacePoint.x = (clipSpacePoint.x / clipSpacePoint.w + 1.0f) * 0.5f * m_viewportWidth;
        screenSpacePoint.y = m_viewportHeight - (1.0f - clipSpacePoint.y / clipSpacePoint.w) * 0.5f * m_viewportHeight;
        return screenSpacePoint;
    }

    glm::vec3 Camera::ScreenToWorld(const glm::vec2& screenPoint) const
    {
        if (m_viewportHeight == 0 || m_viewportWidth == 0) return{ -1,-1,-1 };

        f32 normalizedX = (2.0f * screenPoint.x / m_viewportWidth) - 1.0f;
        f32 normalizedY = 1.0f - (2.0f * (m_viewportHeight - screenPoint.y) / m_viewportHeight);
        glm::vec4 cameraSpacePoint(normalizedX, normalizedY, -1, 1.0f);
        glm::vec4 worldSpacePoint = m_inverseProjection * cameraSpacePoint;
        worldSpacePoint.w = 1.0f;
        worldSpacePoint = m_inverseView * worldSpacePoint;
        return glm::vec3{ worldSpacePoint.x, worldSpacePoint.y, worldSpacePoint.z };
    }

    void Camera::RecalculateProjection()
    {
        m_projection = glm::perspectiveFov(glm::radians(m_verticalFOV), (f32)m_viewportWidth, (f32)m_viewportHeight, m_nearClip, m_farClip);
        m_inverseProjection = glm::inverse(m_projection);
    }

    void Camera::RecalculateView()
    {
        m_view = glm::lookAt(m_position, m_position + m_forwardDirection, glm::vec3(0, 1, 0));
        m_inverseView = glm::inverse(m_view);
    }

    void Camera::RecalculateRayDirections()
    {
        m_rayDirections.resize(m_viewportWidth * m_viewportHeight);

        for (u32 y = 0; y < m_viewportHeight; y++)
        {
            for (u32 x = 0; x < m_viewportWidth; x++)
            {
                glm::vec2 coord = { (f32)x / (f32)m_viewportWidth, (f32)y / (f32)m_viewportHeight };
                coord = coord * 2.0f - 1.0f; // -1 -> 1

                glm::vec4 target = m_inverseProjection * glm::vec4(coord.x, coord.y, -1, 1);
                glm::vec3 rayDirection = glm::vec3(m_inverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0)); // World space
                m_rayDirections[x + y * m_viewportWidth] = rayDirection;
            }
        }
    }
}
