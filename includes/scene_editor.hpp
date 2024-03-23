#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include <xengine/enviroment.hpp>

class SceneEditor {
public:
	enum NewTransformType {
		NTT_EMPTY = 0x0,
		NTT_3D_PLANE = 0x1,
		NTT_3D_CUBE = 0x2,
	};

	static void add_entry(NewTransformType t_type, Material* t_mat) {
		Transform* obj = new Transform();
		switch (t_type) {
		case SceneEditor::NTT_3D_PLANE:
			break;
		case SceneEditor::NTT_3D_CUBE:
			break;
		default:
			break;
		}
		obj->set_material(t_mat);
		obj->initialize();
	}
};

#endif // SCENE_EDITOR_H