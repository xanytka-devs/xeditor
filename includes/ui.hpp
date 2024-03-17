#ifndef UI_H
#define UI_H

#include <xengine/enviroment.hpp>
#include <xengine/app.hpp>
#include <xengine/rendering/material.hpp>

class UI {
public:
	struct UIEditorData {
		XEngine::App* t_app;
		XEngine::Camera* t_camera;
		Cube* t_cube;
		Transform* t_model;
		Scene* t_scene;
	};

	static void init();
	static void set_theme();
	static void draw(UIEditorData t_data);
	static void update_pos(XEngine::Camera* t_camera);
	static bool can_render;
};

#endif // UI_H