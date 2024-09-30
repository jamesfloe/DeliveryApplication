#include "Useimgui.h"
#include "imgui_stdlib.h"
#include "Employee.h"
#include "Delivery.h"
#include "glad/glad.h"
#include "GLFW\glfw3.h"
#include "callBacks.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

bool showEmployeeCreationWindow{ false };
bool showCredentialExists{ false };
bool showCredentialCreated{ false };
bool showEmployeeDeletionWindow{ false };
bool showEmployeeIsDeleted{ false };
bool showLoginWindow{ false };
bool nameOrPassNoExist{ false };
bool showEmployeeDetailsWin{ false };
bool showDeliveryCreationWin{ false };
bool showItemManagerWin{ false };
bool createItemPopUp{ false };
bool showItemDetails{ false };
std::string employeeCreationBuffer{ "enter new Username here" };
std::string passwordCreationBuffer{ "enter new Password here" };
std::string loginUserNameBuffer{ "enter Username" };
std::string loginPasswordBuffer{ "enter password" };
std::string itemNameBuffer{ "enter item Name" };
std::string itemPriceBuffer{ "enter item price" };
static const char* currentListEmployee{ NULL };
static int employeeListIdx;
static const char* currentEmployee{ NULL };
static int employeeIdx;
static const char* currentListDelivery{ NULL };
static int listDeliveryIdx;
static int listItemIdx;
static int listSubItemIdx;
static const char* currentListItemDetails{ NULL };
static const char* currentListItem{ NULL };

void glVertexBufferInt()
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void UseImGUI::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
}

void UseImGUI::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UseImGUI::Update(ImGuiIO& io)
{
    //Employees Window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 4, io.DisplaySize.y));
    ImGui::Begin("Employees", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);
    ImGui::BeginListBox("Employees", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 4));
    for (int n = 0; n < Employee::loggedInEmployees.size(); n++)
    {
        bool is_selected = (currentListEmployee == Employee::loggedInEmployees[n].m_name.c_str());
        if (ImGui::Selectable(Employee::loggedInEmployees[n].m_name.c_str(), is_selected)) {
            currentListEmployee = Employee::loggedInEmployees[n].m_name.c_str();
            employeeListIdx = n;
        }
        if (is_selected) {
            ImGui::SetItemDefaultFocus();
        }
    }
    ImGui::EndListBox();

    ImGui::BeginGroup();
    if (ImGui::Button("Login")) {
        showLoginWindow = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Logout")) {
        Employee::loggedInEmployees.erase(Employee::loggedInEmployees.begin() + employeeListIdx);
    }
    if (ImGui::Button("Employee Details")) {
        showEmployeeDetailsWin = true;
    }
    ImGui::EndGroup();

    ImGui::Text("Deliveries");

    if (ImGui::BeginListBox("Deliveries", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 3))) {
        for (int n = 0; n < Delivery::deliveryList.size(); n++)
        {
            bool is_selected = (currentListDelivery == Delivery::deliveryList[n].m_address.c_str());
            if (ImGui::Selectable(Delivery::deliveryList[n].m_address.c_str(), is_selected)) {
                currentListDelivery = Delivery::deliveryList[n].m_address.c_str();
                listDeliveryIdx = n;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }
    ImGui::BeginGroup();
    if (ImGui::Button("Assign Delivery")) {
        //loggedInEmployees[employeeListIdx].AssignedDeliveries.push_back(std::move(deliveryList[listDeliveryIdx]));
    }
    ImGui::SameLine();
    if (ImGui::Button("Delivery Details")) {

    }
    ImGui::EndGroup();
    ImGui::End();

    ImGui::Begin("Employee Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);
    ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 4, (io.DisplaySize.y * 2) / 3));
    ImGui::SetWindowSize(ImVec2((io.DisplaySize.x / 4) * 3, io.DisplaySize.y / 3));
    if (ImGui::Button("Create new Employee Account"))
        showEmployeeCreationWindow = true;
    if (ImGui::Button("Delete existing Employee Account"))
        showEmployeeDeletionWindow = true;
    ImGui::End();

    //Map Window
    ImGui::Begin("Map Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);
    ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 4, io.DisplaySize.y / 3));
    ImGui::SetWindowSize(ImVec2((io.DisplaySize.x / 4) * 3, io.DisplaySize.y / 3));
    ImGui::End();

    //Restaurant Settings
    ImGui::Begin("Restaurant Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);
    ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 4, 0));
    ImGui::SetWindowSize(ImVec2((io.DisplaySize.x / 4) * 3, io.DisplaySize.y / 3));
    if (ImGui::Button("Create New Delivery"))
        showDeliveryCreationWin = true;
    if (ImGui::Button("Manage Delivery Items"))
        showItemManagerWin = true;
    ImGui::End();

    //Employee Creation Window
    if (!showEmployeeCreationWindow) {
        showCredentialExists = false;
        showCredentialCreated = false;
    }
    if (showEmployeeCreationWindow) {
        ImGui::Begin("Create New Employee", &showEmployeeCreationWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2));
        ImGui::SetWindowSize(ImVec2(io.DisplaySize.x / 4, io.DisplaySize.y / 4));
        ImGui::SetWindowFocus();
        ImGui::InputText("Enter Employee's Name", &employeeCreationBuffer, 0, MyResizeCallback);
        ImGui::InputText("Create Password", &passwordCreationBuffer, ImGuiInputTextFlags_EnterReturnsTrue, MyResizeCallback);
        if (ImGui::Button("Create", ImVec2(ImGui::GetWindowSize().x / 5, ImGui::GetWindowSize().y / 10))) {
            Employee tObj{ employeeCreationBuffer, passwordCreationBuffer, 0 };
            if (!tObj.CheckIfCredentialsExist(tObj, Employee::allEmployees)) {
                showCredentialExists = false;
                Employee::allEmployees.push_back(tObj);
                Employee::SerealizeEmployees(Employee::allEmployees);
                showCredentialCreated = true;
            }
            else {
                showCredentialExists = true;
                showCredentialCreated = false;
            }
        }
        if (showCredentialExists) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "username already exists");
        }
        if (showCredentialCreated) {
            ImGui::TextColored(ImVec4(0.18f, 1.0f, 0.0f, 1.0f), "employee created");
        }
        ImGui::End();
    }

    //Employee Deletion Window
    if (!showEmployeeDeletionWindow)
        showEmployeeIsDeleted = false;
    if (showEmployeeDeletionWindow) {
        ImGui::Begin("Delete Employee", &showEmployeeDeletionWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2));
        ImGui::SetWindowSize(ImVec2(io.DisplaySize.x / 4, io.DisplaySize.y / 4));
        if (ImGui::BeginCombo("Employees", currentEmployee))
        {
            for (int n = 0; n < Employee::allEmployees.size(); n++)
            {
                bool is_selected = (currentEmployee == Employee::allEmployees[n].m_name.c_str());
                if (ImGui::Selectable(Employee::allEmployees[n].m_name.c_str(), is_selected)) {
                    currentEmployee = Employee::allEmployees[n].m_name.c_str();
                    employeeIdx = n;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x / 5, ImGui::GetWindowSize().y / 10))) {
            Employee::allEmployees.erase(Employee::allEmployees.begin() + employeeIdx);
            Employee::SerealizeEmployees(Employee::allEmployees);
            showEmployeeIsDeleted = true;
        }
        if (showEmployeeIsDeleted) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Employee Deleted");
        }
        ImGui::End();
    }

    //Login Window
    if (!showLoginWindow)
        nameOrPassNoExist = false;
    if (showLoginWindow) {
        ImGui::Begin("Login", &showLoginWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2));
        ImGui::SetWindowSize(ImVec2(io.DisplaySize.x / 4, io.DisplaySize.y / 4));
        ImGui::InputText("Enter Username", &loginUserNameBuffer, 0, MyResizeCallback);
        ImGui::InputText("Enter Password", &loginPasswordBuffer, ImGuiInputTextFlags_EnterReturnsTrue, MyResizeCallback);
        if (ImGui::Button("Login Employee")) {
            Employee tobj(loginUserNameBuffer, loginPasswordBuffer, 0);
            for (Employee employee : Employee::allEmployees) {
                if (employee == tobj) {
                    std::time(&employee.loginTime);
                    Employee::loggedInEmployees.push_back(employee);
                    showLoginWindow = false;
                }
                else {
                    nameOrPassNoExist = true;
                }
            }
        }
        if (nameOrPassNoExist) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "username or password do not exist");
        }
        ImGui::End();
    }

    //Item Manager Window
    if (showItemManagerWin) {
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 3, 0));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y));
        ImGui::Begin("Item Management", &showItemManagerWin, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Items");
        ImGui::BeginChild("itemListChild", ImVec2(ImGui::GetWindowWidth() / 3, ImGui::GetWindowHeight() / 1.5));
        ImGui::BeginListBox("##something");
        for (int n = 0; n < Item::allItems.size(); n++)
        {
            bool is_selected = (currentListItem == Item::allItems[n].m_name.c_str());
            if (ImGui::Selectable(Item::allItems[n].m_name.c_str(), is_selected)) {
                currentListItem = Item::allItems[n].m_name.c_str();
                listItemIdx = n;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
        ImGui::EndChild();

        if (showItemDetails) {
            ImGui::Text("Sub-options");
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 8));
            ImGui::BeginChild("itemListChildDetails", ImVec2(ImGui::GetWindowWidth() / 3, ImGui::GetWindowHeight() / 1.5));
            ImGui::BeginListBox("##something");
            for (int n = 0; n < Item::allItems[listItemIdx].childItemVector.size(); n++)
            {
                bool is_selected = (currentListItemDetails == Item::allItems[listItemIdx].childItemVector[n].m_name.c_str());
                if (ImGui::Selectable(Item::allItems[listItemIdx].childItemVector[n].m_name.c_str(), is_selected)) {
                    currentListItemDetails = Item::allItems[listItemIdx].childItemVector[n].m_name.c_str();
                    listSubItemIdx = n;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
            ImGui::EndChild();
        }
        if (ImGui::Button("Create New Item")) {
            createItemPopUp = true;
        }

        if (createItemPopUp) {
            ImGui::InputText("Enter Item Name", &itemNameBuffer, 0, MyResizeCallback);
            ImGui::InputText("Enter Item Price", &itemPriceBuffer, 0, MyResizeCallback);

            if (ImGui::Button("Create")) {
                //Create new item object and save to disk.
                Item tObj(itemNameBuffer, std::stof(itemPriceBuffer));
                if (!tObj.CheckIfCredentialsExist(tObj, Item::allItems)) {
                    showCredentialExists = false;
                    Item::allItems.push_back(tObj);
                    Item::SerealizeItems(Item::allItems);
                    showCredentialCreated = true;
                }
                else {
                    showCredentialExists = true;
                    showCredentialCreated = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                createItemPopUp = false;
            }
            ImGui::SameLine();
            if (showCredentialExists) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Item already exists");
            }
            ImGui::SameLine();
            if (showCredentialCreated) {
                ImGui::TextColored(ImVec4(0.18f, 1.0f, 0.0f, 1.0f), "Item created");
            }
        }

        ImGui::BeginGroup();
        if (ImGui::Button("Show Item Details")) {
            showEmployeeDetailsWin = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Item")) {
            //Delete selected item from disk and memory.
        }
        ImGui::EndGroup();
        ImGui::End();
    }

    //Employee Details Window
    if (showEmployeeDetailsWin) {
        struct tm* tempTimeStr = localtime(&Employee::loggedInEmployees[employeeListIdx].loginTime);
        char* employeeTime = asctime(tempTimeStr);

        ImGui::Begin("Login", &showEmployeeDetailsWin, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2));
        ImGui::SetWindowSize(ImVec2(io.DisplaySize.x / 4, io.DisplaySize.y / 4));
        ImGui::Text("Last login at: ");
        ImGui::Text(employeeTime);
        ImGui::Text("Assigned Deliveries");
        ImGui::BeginListBox("Assigned Deliveries", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() / 3));
        for (int n = 0; n < Employee::loggedInEmployees[employeeListIdx].AssignedDeliveries.size(); n++)
        {
            bool is_selected = (currentListDelivery == Employee::loggedInEmployees[employeeListIdx].AssignedDeliveries[listDeliveryIdx].m_address);
            if (ImGui::Selectable(Employee::loggedInEmployees[employeeListIdx].AssignedDeliveries[listDeliveryIdx].m_address.c_str(), is_selected)) {
                currentListDelivery = Employee::loggedInEmployees[n].m_name.c_str();
                listDeliveryIdx = n;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
        if (ImGui::Button("close")) {

        }
        ImGui::End();
    }
}

void UseImGUI::Render(GLFWwindow* window)
{
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void UseImGUI::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

