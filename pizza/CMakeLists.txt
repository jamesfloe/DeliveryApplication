# CMakeList.txt : CMake project for pizza, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (pizza "pizza.cpp" "pizza.h" "Useimgui.cpp" "Useimgui.h" "callBacks.h" "Employee.h" "Employee.cpp" "Delivery.h" "Delivery.cpp" "Item.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET pizza PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
find_package(glad CONFIG REQUIRED)
target_link_libraries(pizza PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(pizza PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(pizza PRIVATE imgui::imgui)

find_package(cereal CONFIG REQUIRED)
target_link_libraries(pizza PRIVATE cereal::cereal)