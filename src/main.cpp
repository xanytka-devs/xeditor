#include <string>
#include <vector>
#include <iostream>

#include <xengine/app.hpp>
#include <xengine/common.hpp>
#include <xengine/enviroment.hpp>
#include <xengine/utils.hpp>
#include <../packages/xengine.audio/includes/audio.hpp>
#include <xengine/input.hpp>
#include <xengine/rendering/renderer.hpp>
#include <xengine/rendering/camera.hpp>
#include <xengine/rendering/window.hpp>
#include <xengine/rendering/texture.hpp>
#include <xengine/rendering/light.hpp>
#include <xengine/physics/rigidbody.hpp>
#include <xengine/physics/collider.hpp>
#include <xengine/rendering/cubemap.hpp>

#include "components/light_cube.hpp"
#include "defaults.hpp"

#ifdef XENGINE_GUI
#include "ui.hpp"
#endif // XENGINE_GUI

using namespace XEngine;

Joystick main_j;
Transform model(glm::vec3(0.f), glm::vec4(glm::vec3(0.f), 1.f), glm::vec3(0.05f), "Model");
glm::vec3 point_light_positions[] = {
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  1.0f, -2.5f),
};
const int point_lights_amount = (int)(sizeof(point_light_positions) / sizeof(glm::vec3));
Cube lights = Cube(glm::vec3(0.7f, 0.2f, 2.0f), glm::vec4(0, 0, 0, 1), glm::vec3(0.125f), glm::vec4(0.f, 0.5f, 0.5f, 1.f));
Camera camera(glm::vec3(-1.8f, 3.07f, 3.82f), -56.f, -36.f);
DirectionalLight dir_light = { glm::vec3(-0.2f, -1.f, -0.3f),
    glm::vec4(0.0f), glm::vec4(1.4f), glm::vec4(0.75f), glm::vec4(0.7f, 0.5f, 0.35f, 1.0f) };
SpotLight spot_light = { camera.position, camera.forward, glm::cos(glm::radians(12.5f)),
    glm::cos(glm::radians(20.f)), 1.0f, 0.07f, 0.032f, glm::vec4(0.f), glm::vec4(1.f),
    glm::vec4(1.f), glm::vec4(1.f) };
Audio a{ "res\\sound.wav", false, {"test", 100.f, 2.f}};
Material unlit_mat;
Shader sky_shader;
Cubemap skybox;

int LightSource::global_id = 0;
unsigned char Enviroment::scene_id = 0;
SceneManager Enviroment::scene_manager = SceneManager();
char** passed_args;
int arg_count;

class EditorApp : public App {

    virtual void initiazile() override {
#ifndef NDEBUG
        printf("\nXEditor\nDebug Output\n\n");
#endif
        //Print renderer summary.
        window.set_param(W_VSYNC, true);
        Renderer::print_host_info();
        //Add scene.
        Enviroment::scene_manager.add_scene(Scene("Example scene", &camera));
        //Initialize component system.
        ComponentRegistry::instance().push<LightSource>();
        ComponentRegistry::instance().push<BoxCollider>();
        ComponentRegistry::instance().push<SphereCollider>();
        ComponentRegistry::instance().push<Rigidbody>();
        //Initialize audio manager.
        AudioManager::initialize();
        AudioManager::print_host_info();
        if(arg_count > 1)
            if(std::string(passed_args[1])._Equal("--no_audio")) AudioManager::remove();
        //Skybox.
        skybox.initialize(50);
        skybox.load("res\\skybox\\sh.cubemap");
        over_init();
        //Joystick checks.
        main_j.update();
        //Initialize ImGui.
        window.gui_initialize();
#ifdef XENGINE_GUI
        UI::init();
        UI::update_pos(&camera);
        UI::set_theme();
#endif // XENGINE_GUI
    }

    void over_init() {
        //Model.
        xe_def::model_mat.shader = Shader("res\\materials\\object_vert.glsl", "res\\materials\\object_frag.glsl");
        model.load_model("res\\sphere\\scene.gltf");
        model.set_material(&xe_def::model_mat);
        model.add_component<Rigidbody>(1.0f, glm::vec3(0.0f), glm::vec3(0.0f), false);
        model.add_component<SphereCollider>();
        model.initialize();
        //Light source.
        unlit_mat.shader = Shader("res\\materials\\object_vert.glsl", "res\\materials\\unlit_frag.glsl");
        lights.set_material(&unlit_mat);
        lights.add_component<LightSource>();
        lights.get_component<LightSource>().light.ambient = glm::vec4(0.25f);
        for(unsigned int i = 0; i < point_lights_amount; i++)
            lights.create_instance(point_light_positions[i], glm::vec4(0, 0, 0, 1), glm::vec3(0.125f));
        lights.initialize();
        //Cubemap.
        sky_shader = Shader("res\\materials\\skybox\\skybox_vert.glsl", "res\\materials\\skybox\\skybox_frag.glsl");
        model.set_cubemap(skybox.get_id());
    }

    bool is_reloading = false;

    void hot_reload() {
        is_reloading = true;
#ifndef NDEBUG
        printf("\n[INFO] Using hot reload...\n");
#endif
        over_shut();
        over_init();
#ifndef NDEBUG
        printf("[INFO] Hot reload complete.\n");
#endif
        is_reloading = false;
    }

    int mode = 0;
    bool flashlight = false;
	virtual void update() override {
        if(is_reloading) return;
        if(window.height <= 0) return;
        //Collisions.
        CollissionSystem::instance().update();
        //Take care of input.
        input();
        //Create transformations.
        camera.aspect = static_cast<float>(window.width) / static_cast<float>(window.height);
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = camera.get_projection_matrix();
        //Light.
        dir_light.direction = glm::vec3(
            glm::rotate(glm::mat4(1.f), glm::radians(0.1f), glm::vec3(1.f, 0.f, 0.f)) *
            glm::vec4(dir_light.direction, 1.f)
        );
        //Directional light.
        dir_light.render(xe_def::model_mat.shader);
        if(!flashlight) {
            //Point light.
            lights.render_light(xe_def::model_mat.shader);
            xe_def::model_mat.shader.set_int("num_point_lights", LightSource::global_id);
            //Spot light.
            xe_def::model_mat.shader.set_int("num_spot_lights", 0);
        } else {
            //Point light.
            xe_def::model_mat.shader.set_int("num_point_lights", 0);
            //Spot light.
            spot_light.position = camera.position;
            spot_light.direction = camera.forward;
            spot_light.render(xe_def::model_mat.shader, 0);
            xe_def::model_mat.shader.set_int("num_spot_lights", 1);
        }
        //Model.
        xe_def::model_mat.shader.enable();
        xe_def::model_mat.shader.set_4_floats("material.diffuse", xe_def::model_mat.diffuse);
        xe_def::model_mat.shader.set_4_floats("material.specular", xe_def::model_mat.specular);
        xe_def::model_mat.shader.set_4_floats("material.emission", xe_def::model_mat.emission);
        xe_def::model_mat.shader.set_4_floats("material.emission_color", xe_def::model_mat.emission_color);
        xe_def::model_mat.shader.set_float("material.shininess", xe_def::model_mat.shininess);
        xe_def::model_mat.shader.set_float("material.emission_factor", xe_def::model_mat.emission_factor);
        xe_def::model_mat.shader.set_int("render_mode", mode);
        xe_def::model_mat.shader.set_int("use_skybox", 1);
        xe_def::model_mat.shader.set_float("material.skybox_refraction", xe_def::model_mat.skybox_refraction);
        xe_def::model_mat.shader.set_float("material.skybox_refraction_strength",
            xe_def::model_mat.skybox_refraction_strength);
        //Render light.
        unlit_mat.shader.enable();
        unlit_mat.shader.set_int("render_mode", mode);
        unlit_mat.shader.set_4_floats("color", lights.color);
        //Render all transforms.
        for(Transform* t : Enviroment::get_current_scene()->transforms)
            t->render();
        //Skybox.
        skybox.render(sky_shader);
#ifdef XENGINE_GUI
        //UI rendering.
        UI::draw({ this, &camera, Enviroment::get_current_scene(), &xe_def::model_mat });
#endif // XENGINE_GUI
    }

    bool clicked = false;
    float accel = 1.f;
    void input() {
        //On 'Esc' close app.
        if(Keyboard::key_state(KeyCode::ESCAPE) || main_j.button_state(JoystickControls::J_HOME))
            App::shutdown();
        //Hot reloading.
        if(Keyboard::key_state(KeyCode::R)) hot_reload();
        //Default mode.
        if(Keyboard::key_down(KeyCode::KEY_1) || main_j.button_state(JoystickControls::TRIGGER_LEFT)) { mode = 0;
            Renderer::switch_mode(RenderMode::DEFAULT);
        }
        //Wireframe mode.
        if(Keyboard::key_down(KeyCode::KEY_2) || main_j.button_state(JoystickControls::TRIGGER_RIGHT)) { mode = 0;
            Renderer::switch_mode(RenderMode::WIREFRAME);
        }
        //UV mode.
        if(Keyboard::key_down(KeyCode::KEY_3)) { mode = 1;
            Renderer::switch_mode(RenderMode::DEFAULT);
        }
        //Normal mode.
        if(Keyboard::key_down(KeyCode::KEY_4)) { mode = 2;
            Renderer::switch_mode(RenderMode::DEFAULT);
        }
        //Movement.
        if(Keyboard::key_down(KeyCode::T))
            model.get_component<Rigidbody>().add_force(FT_Force, glm::vec3(0.f, 1.f, 0.f));
        float j_x = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_X);
        float j_y = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_Y);
        float mouse_dy = static_cast<float>(Mouse::get_wheel_dy());
        //Check if RMB is pressed.
        if(Mouse::button_down(1)) clicked = !clicked;
        if(clicked) {
            float dt = Enviroment::delta_time;
            window.set_param(W_CURSOR, C_DISABLED);
            //Position changes.
            //Acceleration.
            if(Keyboard::key_state(KeyCode::LEFT_CONTROL) || Keyboard::key_state(KeyCode::RIGHT_CONTROL))
                accel += 0.001f;
            else accel = 1.f;
            // F/B movement.
            if(Keyboard::key_state(KeyCode::W) || j_y <= -0.5f)
                camera.position += camera.forward * (dt * 2.5f) * accel;
            if(Keyboard::key_state(KeyCode::S) || j_y >= 0.5f)
                camera.position -= camera.forward * (dt * 2.5f) * accel;
            // R/L movement.
            if(Keyboard::key_state(KeyCode::D) || j_x >= 0.5f)
                camera.position += camera.right * (dt * 2.5f) * accel;
            if(Keyboard::key_state(KeyCode::A) || j_x <= -0.5f)
                camera.position -= camera.right * (dt * 2.5f) * accel;
            // U/D movement.
            if(Keyboard::key_state(KeyCode::SPACEBAR) || main_j.button_state(JoystickControls::DPAD_UP))
                camera.position += glm::vec3(0.f, 1.f, 0.f) * (dt * 2.5f);
            if(Keyboard::key_state(KeyCode::LEFT_SHIFT) || Keyboard::key_state(KeyCode::RIGHT_SHIFT)
                || main_j.button_state(JoystickControls::DPAD_DOWN))
                camera.position -= glm::vec3(0.f, 1.f, 0.f) * (dt * 2.5f);
            //Camera rotation.
            double dx = Mouse::get_cursor_dx(), dy = Mouse::get_cursor_dy();
            if(dx != 0 || dy != 0) rotate_camera(dx, dy);
            else if(main_j.is_present()) {
                dx = main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_X);
                dy = -main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_Y);
                if(dx >= 0.5 || dy >= 0.5 || dx <= -0.5 || dy <= -0.5)
                    rotate_camera(dx, dy);
            }
            //Move with mouse wheel.
            if(mouse_dy != 0)
                camera.position += camera.forward * (dt * mouse_dy * 2.5f);
#ifdef XENGINE_GUI
            //Update UI.
            UI::update_pos(&camera);
#endif // XENGINE_GUI
        } else {
            window.set_param(W_CURSOR, C_NONE);
            if(Keyboard::key_state(KeyCode::LEFT_SHIFT) || Keyboard::key_state(KeyCode::RIGHT_SHIFT)) {
                //Camera zoom.
                camera.fov -= mouse_dy;
                if (camera.fov < 1.f) camera.fov = 180.f;
                else if (camera.fov > 180.f) camera.fov = 1.f;
            }
        }
        if(Keyboard::key_down(KeyCode::P))
            window.set_param(WindowParam::W_TITLE, "Hello XEngine!");
        //Lighting.
        if(Keyboard::key_down(KeyCode::F) || main_j.button_state(JoystickControls::BTN_UP)) {
            flashlight = !flashlight;
            if(flashlight) Renderer::set_clear_color(glm::vec3(0.f, 0.f, 0.f));
            else Renderer::set_clear_color(glm::vec3(0.15f, 0.15f, 0.15f));
        }
        //Sound button.
        if(Keyboard::key_down(KeyCode::M) || main_j.button_state(JoystickControls::BTN_DOWN))
            a.play();
        //Update joystick.
        main_j.update();
    }

    void rotate_camera(double dz, double dy) {
        camera.rotation.z += static_cast<float>(dz);
        if(camera.rotation.z >= 360.f) camera.rotation.z -= 360.f;
        if(camera.rotation.z <= -360.f) camera.rotation.z += 360.f;
        camera.rotation.y += static_cast<float>(dy);
        if(camera.rotation.y > 89.f) camera.rotation.y = 89.f;
        else if(camera.rotation.y < -89.f) camera.rotation.y = -89.f;
        camera.update_vectors();
    }

    virtual void on_shutdown() override {
        over_shut();
        AudioManager::remove();
        window.gui_shutdown();
        Utils::write_file("window_config.ini",
            "[Window][XEditor]\nSize=" + std::to_string(window.width) + "," + std::to_string(window.height));
    }

    void over_shut() {
        for(Transform* t : Enviroment::get_current_scene()->transforms)
            t->remove();
        xe_def::model_mat.shader.remove();
        unlit_mat.shader.remove();
        LightSource::reset_global_id();
    }
};

int main(int argc, char** argv) {
    //Entire application startup.
    arg_count = argc;
    passed_args = argv;
    //Get saved window size.
    std::vector<std::string> w_size = { "1300", "800" };
    std::string file = Utils::read_from_file("window_config.ini");
    if (file != "") {
        std::vector<std::string> content = Utils::split_str(&file, '\n');
        w_size = Utils::split_str(&(Utils::split_str(&content[1], '=')[1]), ',');
    }
    //Create instance of app.
    auto editor = std::make_unique<EditorApp>();
    int returnC = editor->start(std::stoi(w_size[0]), std::stoi(w_size[1]), "XEditor");
    return 0;
}