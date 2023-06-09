cmake_minimum_required(VERSION 3.5)
project(navigate_bt)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CONFIG_PATH}")

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(dialogflow_ros2_interfaces REQUIRED)
find_package(gb_dialog REQUIRED)
find_package(action_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(std_msgs REQUIRED)

find_package(ZMQ)
if(ZMQ_FOUND)
    message(STATUS "ZeroMQ found.")
    add_definitions(-DZMQ_FOUND)
else()
  message(WARNING "ZeroMQ NOT found. Not including PublisherZMQ.")
endif()

set(CMAKE_CXX_STANDARD 17)

set(dependencies
  rclcpp
  rclcpp_action
  behaviortree_cpp_v3
  dialogflow_ros2_interfaces
  gb_dialog
  action_msgs
  geometry_msgs
  nav2_msgs
  ament_index_cpp
  std_msgs
)

include_directories(include ${ZMQ_INCLUDE_DIRS})


add_library(request_name SHARED src/navigate_bt/RequestName.cpp)
add_library(introduction SHARED src/navigate_bt/Introduction.cpp)
list(APPEND plugin_libs
  request_name
  introduction

  
)

foreach(bt_plugin ${plugin_libs})
  ament_target_dependencies(${bt_plugin} ${dependencies})
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()

add_executable(navigate_main src/navigate_main.cpp)
ament_target_dependencies(navigate_main ${dependencies})
target_link_libraries(navigate_main ${ZMQ_LIBRARIES})

install(TARGETS
  ${plugin_libs}
  navigate_main
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

install(DIRECTORY behavior_tree_xml launch
  DESTINATION share/${PROJECT_NAME}
)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

  find_package(ament_cmake_gtest REQUIRED)

endif()

ament_export_include_directories(include)
ament_export_dependencies(${dependencies})

ament_package()
