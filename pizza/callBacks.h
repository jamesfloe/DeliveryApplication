#pragma once
#include<GLFW/glfw3.h>
#include<imgui.h>
#include <string>
#include <fstream>

static void glfw_error_callback(int error, const char* description) {
	throw("glfw error");
	fprintf(stderr, "Error: %s\n", description);
}
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
static int MyResizeCallback(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        ImVector<char>* my_str = (ImVector<char>*)data->UserData;
        IM_ASSERT(my_str->begin() == data->Buf);
        my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
        data->Buf = my_str->begin();
    }
    return 0;
}
