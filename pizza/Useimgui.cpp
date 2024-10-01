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

enum msgState {
    none = 0,
    exists = 1,
    created = 2,
    deleted = 3,
    reEnter = 4
};
msgState itemMsg;
msgState empMsg;
msgState optMsg;
bool showEmployeeCreationWindow{ false };
bool showEmployeeCredentialExists{ false };
bool showEmployeeCredentialCreated{ false };
bool showOptionEnterCorrectNum{ false };
bool showItemEnterCorrectNum{ false };
bool showItemCredentialExists{ false };
bool showItemCredentialCreated{ false };
bool showOptionCredentialExists{ false };
bool showOptionCredentialCreated{ false };
bool showEmployeeDeletionWindow{ false };
bool showEmployeeIsDeleted{ false };
bool showLoginWindow{ false };
bool nameOrPassNoExist{ false };
bool showEmployeeDetailsWin{ false };
bool showDeliveryCreationWin{ false };
bool showItemManagerWin{ false };
bool createItemPopUp{ false };
bool showItemDetails{ false };
bool createOptionPopUp{ false };
bool showOptionIsDeleted{ false };
bool showItemIsDeleted{ false };
std::string employeeCreationBuffer{ "enter new Username here" };
std::string passwordCreationBuffer{ "enter new Password here" };
std::string loginUserNameBuffer{ "enter Username" };
std::string loginPasswordBuffer{ "enter password" };
std::string itemNameBuffer{ "enter item Name" };
std::string itemPriceBuffer{ "enter item price" };
std::string optionNameBuffer{ "enter option name" };
std::string optionPriceBuffer{ "enter option price" };
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
        showEmployeeCredentialExists = false;
        showEmployeeCredentialCreated = false;
        empMsg = none;
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
                empMsg = none;
                Employee::allEmployees.push_back(tObj);
                Employee::SerealizeEmployees(Employee::allEmployees);
                empMsg = created;
            }
            else {
                empMsg = exists;
            }
        }
        if (empMsg == exists) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "username already exists");
        }
        if (empMsg == created) {
            ImGui::TextColored(ImVec4(0.18f, 1.0f, 0.0f, 1.0f), "employee created");
        }
        ImGui::End();
    }

    //Employee Deletion Window
    if (!showEmployeeDeletionWindow)
        empMsg = none;
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
            empMsg = deleted;
        }
        if (empMsg == deleted) {
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
    if (showItemManagerWin == false) {
        showItemDetails = false;
        itemMsg = none;
        optMsg = none;
    }
    if (showItemManagerWin) {
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 3, 0));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y));
        ImGui::Begin("Item Management", &showItemManagerWin, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::BeginChild("itemListChild", ImVec2(ImGui::GetWindowWidth() / 3, ImGui::GetWindowHeight()));
        ImGui::Text("Items");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
        std::string itemPrice{ "Item price is: " };
        itemPrice = itemPrice + (Item::allItems.size() != 0 ? std::to_string(Item::allItems[listItemIdx].m_price): "");
        ImGui::Text(itemPrice.c_str());
        ImVec2 prevListBoxHeight{ ImGui::GetWindowPos()}; // Used to set height of SubOptions childWindow
        ImGui::BeginListBox("##something", ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2));
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

        if (ImGui::Button("Create New Item")) {
            createItemPopUp = true;
        }

        if (createItemPopUp) {
            ImGui::InputText("Enter Item Name", &itemNameBuffer, 0, MyResizeCallback);
            ImGui::InputText("Enter Item Price", &itemPriceBuffer, 0, MyResizeCallback);

            if (ImGui::Button("Create")) {
                //Create new item object and save to disk.
                if (isNumberAndPeriod(itemPriceBuffer)) {
                    Item tObj(itemNameBuffer, std::stof(itemPriceBuffer));
                    if (!tObj.CheckIfCredentialsExist(tObj, Item::allItems)) {
                        itemMsg = none;
                        Item::allItems.push_back(tObj);
                        Item::SerealizeItems(Item::allItems);
                        itemMsg = created;
                    }
                    else {
                        itemMsg = exists;
                    }
                }
                else {
                    itemMsg = reEnter;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                createItemPopUp = false;
                itemMsg = none;
            }
            //ImGui::SameLine();
            if (itemMsg == exists) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Item already exists");
            }
            //ImGui::SameLine();
            if (itemMsg == created) {
                ImGui::TextColored(ImVec4(0.18f, 1.0f, 0.0f, 1.0f), "Item created");
            }
            //ImGui::SameLine();
            if (itemMsg == reEnter) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Please enter price correctly");
            }
        }

        ImGui::BeginGroup();
        if (ImGui::Button("Show Item Details") && Item::allItems.size() != 0) {
            showItemDetails = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Item")) {
            //Delete selected item from disk and memory.
            if (Item::allItems.size() != 0) {
                Item::allItems.erase(Item::allItems.begin() + listItemIdx);
                Item::SerealizeItems(Item::allItems);
                listItemIdx > 0 ? listItemIdx -= 1 : listItemIdx = listItemIdx;
                itemMsg = deleted;
            }
        }
        if (itemMsg == deleted) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Item Deleted");
        }
        ImGui::EndGroup();
        ImGui::EndChild();

        if (showItemDetails) {
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2, prevListBoxHeight.y));
            ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight()));
            ImGui::BeginChild("itemListChildDetails", ImVec2(ImGui::GetWindowWidth() / 3, ImGui::GetWindowHeight()));
            ImGui::Text("Sub-options");
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
            std::string optionPrice{ "Option price is: " };
            optionPrice = optionPrice + (Item::allItems[listItemIdx].childItemVector.size() != 0 ? std::to_string(Item::allItems[listItemIdx].childItemVector[listSubItemIdx].m_price) : "");
            ImGui::Text(optionPrice.c_str());
            ImGui::BeginListBox("##something", ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2));
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
            if (ImGui::Button("Create New Option")) {
                createOptionPopUp = true;
            }

            if (createOptionPopUp) {
                ImGui::InputText("Enter Option Name", &optionNameBuffer, 0, MyResizeCallback);
                ImGui::InputText("Enter Option Price", &optionPriceBuffer, 0, MyResizeCallback);

                if (ImGui::Button("Create")) {
                    //Create new item option and update object to save to disk
                    if (isNumberAndPeriod(optionPriceBuffer)) {
                        Item tObj(optionNameBuffer, std::stof(optionPriceBuffer));
                        if (!tObj.CheckIfCredentialsExist(tObj, Item::allItems[listItemIdx].childItemVector)) {
                            optMsg = none;
                            Item::allItems[listItemIdx].childItemVector.push_back(tObj);
                            Item::SerealizeItems(Item::allItems);
                            optMsg = created;
                        }
                        else {
                            optMsg = exists;
                        }
                    }
                    else {
                        optMsg = reEnter;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Close")) {
                    createOptionPopUp = false;
                    optMsg = none;
                }
                //ImGui::SameLine();
                if (optMsg == exists) {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Option already exists");
                }
                //ImGui::SameLine();
                if (optMsg == created) {
                    ImGui::TextColored(ImVec4(0.18f, 1.0f, 0.0f, 1.0f), "Option created");
                }
                //ImGui::SameLine();
                if (optMsg == deleted) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Option Deleted");
                }
                //ImGui::SameLine();
                if (optMsg == reEnter) {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Please enter price correctly");
                }
            }

            if (ImGui::Button("Delete Option")) {
                //Delete selected Option from disk and memory.
                if (Item::allItems[listItemIdx].childItemVector.size() != 0) {
                Item::allItems[listItemIdx].childItemVector.erase(Item::allItems[listItemIdx].childItemVector.begin() + listSubItemIdx);
                Item::SerealizeItems(Item::allItems);
                listSubItemIdx > 0 ? listSubItemIdx -= 1 : listSubItemIdx = listSubItemIdx;
                optMsg = deleted;
                }
            }
            ImGui::EndChild();
        }
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

