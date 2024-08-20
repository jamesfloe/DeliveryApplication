#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include <string>

class UseImGUI {
public:
	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	virtual void Update(ImGuiIO& io);
	void Render(GLFWwindow* window);
	void Shutdown();
};

class UseImGUIMap : public UseImGUI {
public:
	virtual void Update(ImGuiIO& io) override {

	}
};