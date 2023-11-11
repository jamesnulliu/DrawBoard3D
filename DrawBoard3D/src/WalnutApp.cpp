#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

#include "Tools/Logger.hpp"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		static int count = 0;
		ImGui::Begin("Hello");
		if (ImGui::Button("Button")) {
			DB3D_TRACE("Hey! Button has been pressend {} times", ++count);
		}
		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "DrawBoard3D";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}