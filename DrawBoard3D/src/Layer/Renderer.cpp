#include "Renderer.hpp"
#include "Tools/Utils.hpp"
#include "glm/glm.hpp"
#include "Tools/Logger.hpp"
#include "Tools/Color.hpp"
#include <algorithm>

namespace db3d {
    Renderer::Renderer() : m_finalImage(nullptr), m_imageData()
    {
    }

    void Renderer::OnResize(u64 width, u64 height)
    {
        // If `m_finalImage` already exists:
        if (m_finalImage != nullptr) {
            m_finalImage->Resize(u32(width), u32(height));
        }
        // Or `m_finalImage` does not exist:
        else {
            m_finalImage = std::make_shared<Walnut::Image>(u32(width), u32(height), Walnut::ImageFormat::RGBA);
        }
        // Row: [0, height), Col: [0, width)
        m_imageData.resize(width, height);
    }

    void Renderer::Render(std::shared_ptr<Scene> activeScene)
    {
		if (activeScene == nullptr) {
			DB3D_WARNING("No active scene, rendered nothing.");
            return;
		}

        // Set the background color to black
        m_imageData.fill(0);

        glm::vec<2, u64> lb{}, rt{};
        lb.x = lb.y = 0;
        rt.x = m_imageData.width / 2;
        rt.y = m_imageData.height / 2;

        for (u64 x = lb.x; x < rt.x; ++x) {
            for (u64 y = lb.y; y < rt.y; ++y) {
                m_imageData[To1DIndex(glm::vec2{ x, y }, m_imageData.width)] = agbr::BROWN;
            }
        }

        auto& entities = activeScene->entities;

        m_finalImage->SetData(m_imageData.raw);
    }
}
