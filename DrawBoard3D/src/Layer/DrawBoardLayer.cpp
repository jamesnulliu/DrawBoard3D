#include "DrawBoardLayer.hpp"

namespace db3d {
	DrawBoardLayer::DrawBoardLayer()
		: Walnut::Layer(), m_imageWidth(0), m_imageHeight(0)
	{
		m_scenes.push_back(std::make_shared<Scene>());
		m_activeScene = m_scenes.front();
	}

	void DrawBoardLayer::OnUIRender()
	{
		static int count = 0;
		ImGui::Begin("Hello");
		if (ImGui::Button("Button")) {
			DB3D_WARNING("Hey! Button has been pressed for {} times", ++count);
		}
		ImGui::End();

		ShowSceneWindow();
	}

	void DrawBoardLayer::Render()
	{
		if (m_activeScene == nullptr) {
			DB3D_WARNING("No active scene, rendered nothing.");
            return;
		}

		u64 curWidth = m_renderer.GetImageWidth();
		u64 curHeight = m_renderer.GetImageHeight();
		auto finalImage = m_renderer.GetFinalImage();

		if (finalImage == nullptr || curWidth != m_imageWidth || curHeight != m_imageHeight) {
			m_renderer.OnResize(m_imageWidth, m_imageHeight);
			for (const auto& camera : m_activeScene->cameras) {
				camera->OnResize(m_imageWidth, m_imageHeight);
			}
		}

		m_renderer.Render(m_activeScene);
	}

	void DrawBoardLayer::ShowSceneWindow()
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Scene");
		m_imageWidth = u64(ImGui::GetContentRegionAvail().x);
		m_imageHeight = u64(ImGui::GetContentRegionAvail().y);

		if (m_imageWidth == 0 || m_imageHeight == 0) { return; }

		Render();  // Render the image based on image width and height.

		auto finalImage = m_renderer.GetFinalImage();
		if (finalImage == nullptr) {
			DB3D_WARNING("No final image, showed nothing.");
		} else {
			ImGui::Image(
				finalImage->GetDescriptorSet(),
				{ f32(finalImage->GetWidth()), f32(finalImage->GetHeight()) },
				{ 0, 1 },
				{ 1, 0 }
			);
		}
		ImGui::End();
        ImGui::PopStyleVar();
	}
}
