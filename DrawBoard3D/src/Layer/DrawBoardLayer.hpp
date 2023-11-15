#pragma once
#include "Walnut/Application.h"
#include "Tools/Logger.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"

namespace db3d {
	class DrawBoardLayer : public Walnut::Layer {
	public:
		explicit DrawBoardLayer();

		virtual void OnUIRender() override;
		void Render();

		void ShowSceneWindow();

	private:
		u64 m_imageWidth;
		u64	m_imageHeight;
		Renderer m_renderer;
		std::shared_ptr<Scene> m_activeScene;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
