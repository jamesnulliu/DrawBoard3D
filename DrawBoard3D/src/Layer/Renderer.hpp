#pragma once
#include "Essentials.hpp"
#include <memory>
#include <vector>
#include <array>
#include "Walnut/Image.h"
#include "Scene.hpp"
#include "ImageData.hpp"

namespace db3d {

    class Renderer
    {

    public:
        explicit Renderer();

        void OnResize(u64 width, u64 height);
        void Render(std::shared_ptr<Scene> activeScene);

        std::shared_ptr<const Walnut::Image> GetFinalImage() const { return m_finalImage; }
        u64 GetImageWidth() const { return m_imageData.width; }
        u64 GetImageHeight() const { return m_imageData.height; }
        const auto& GetImageRange() const { return m_imageData.range; }

    private:
        std::shared_ptr<Walnut::Image> m_finalImage;
        ImageData m_imageData;
    };
}
