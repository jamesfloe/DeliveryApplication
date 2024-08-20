#include <vector>
#include "pizza.h"
#include "Useimgui.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "callBacks.h"
#include "Employee.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

using namespace std;

int main()
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window{ glfwCreateWindow(1920, 1080, "PizzaHelper", nullptr, nullptr) };
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	UseImGUI myimgui;
	myimgui.Init(window, glsl_version);
	Employee::DeserealizeEmployeesToVec(Employee::allEmployees);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		myimgui.NewFrame();
		ImGuiIO& io = ImGui::GetIO();
		myimgui.Update(io);
		myimgui.Render(window);
	}
	myimgui.Shutdown();
	glfwTerminate();
}
