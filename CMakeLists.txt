cmake_minimum_required(VERSION 3.12)
project(xeditor)
set(EDITOR_PROJECT_NAME xeditor)
add_subdirectory(external/xengine)

file(GLOB_RECURSE EDITOR_INCLUDES "includes/*.hpp")
file(GLOB_RECURSE EDITOR_SOURCES "src/*.cpp")

set(EDITOR_ALL
	${EDITOR_SOURCES}
	${EDITOR_INCLUDES}
)

add_executable(${EDITOR_PROJECT_NAME}
	${EDITOR_ALL}
)

target_link_libraries(${EDITOR_PROJECT_NAME} xengine ImGui glm)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC includes)
target_include_directories(${EDITOR_PROJECT_NAME} PRIVATE src)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT xeditor)
set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)