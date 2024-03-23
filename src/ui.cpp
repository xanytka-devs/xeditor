#include <imgui.h>
#include <imgui_internal.h>
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include <xengine/rendering/renderer.hpp>
#include <xengine/rendering/camera.hpp>
#include <../packages/xengine.io/includes/os.hpp>
#include <xengine/physics/rigidbody.hpp>

#include "components/light_cube.hpp"
#include "scene_editor.hpp"
#include "ui.hpp"
#include <xengine/physics/collider.hpp>

void UI::init() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("res\\consola.ttf", 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
}

void UI::set_theme() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Color palette
    ImVec4 _black = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    ImVec4 _white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    ImVec4 _grey = ImVec4(0.60f, 0.60f, 0.60f, 0.35f);
    ImVec4 _dark = ImVec4(0.08f, 0.08f, 0.08f, 0.75f);
    ImVec4 _darkgrey = ImVec4(0.23f, 0.23f, 0.23f, 0.35f);
    ImVec4 _theme_clr = ImVec4(1.f, 0.5f, 0.f, 1.f);
    ImVec4 _light_theme_clr = ImVec4(1.f, 0.6f, 0.f, 1.f);
    ImVec4 _dark_theme_clr = ImVec4(0.7f, 0.3f, 0.f, 1.f);
    ImVec4 _lighgrey = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

    // Color array
    style.Colors[ImGuiCol_Text] = _white;
    style.Colors[ImGuiCol_TextDisabled] = _grey;
    style.Colors[ImGuiCol_WindowBg] = _dark;
    style.Colors[ImGuiCol_ChildBg] = _dark;
    style.Colors[ImGuiCol_PopupBg] = _dark;
    style.Colors[ImGuiCol_Border] = _grey;
    style.Colors[ImGuiCol_BorderShadow] = _black;
    style.Colors[ImGuiCol_FrameBg] = _darkgrey;
    style.Colors[ImGuiCol_FrameBgHovered] = _dark_theme_clr;
    style.Colors[ImGuiCol_FrameBgActive] = _grey;
    style.Colors[ImGuiCol_TitleBg] = _darkgrey;
    style.Colors[ImGuiCol_TitleBgActive] = _darkgrey;
    style.Colors[ImGuiCol_TitleBgCollapsed] = _darkgrey;
    style.Colors[ImGuiCol_MenuBarBg] = _darkgrey;
    style.Colors[ImGuiCol_ScrollbarBg] = _darkgrey;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = _grey;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = _grey;
    style.Colors[ImGuiCol_CheckMark] = _lighgrey;
    style.Colors[ImGuiCol_SliderGrab] = _lighgrey;
    style.Colors[ImGuiCol_SliderGrabActive] = _white;
    style.Colors[ImGuiCol_Button] = _darkgrey;
    style.Colors[ImGuiCol_ButtonHovered] = _grey;
    style.Colors[ImGuiCol_ButtonActive] = _darkgrey;
    style.Colors[ImGuiCol_Header] = _darkgrey;
    style.Colors[ImGuiCol_HeaderHovered] = _grey;
    style.Colors[ImGuiCol_HeaderActive] = _grey;
    style.Colors[ImGuiCol_Separator] = _grey;
    style.Colors[ImGuiCol_SeparatorHovered] = _theme_clr;
    style.Colors[ImGuiCol_SeparatorActive] = _light_theme_clr;
    style.Colors[ImGuiCol_ResizeGrip] = _darkgrey;
    style.Colors[ImGuiCol_ResizeGripHovered] = _theme_clr;
    style.Colors[ImGuiCol_ResizeGripActive] = _light_theme_clr;
    style.Colors[ImGuiCol_Tab] = _theme_clr;
    style.Colors[ImGuiCol_TabHovered] = _light_theme_clr;
    style.Colors[ImGuiCol_TabActive] = _theme_clr;
    style.Colors[ImGuiCol_TabUnfocused] = _grey;
    style.Colors[ImGuiCol_TabUnfocused] = _grey;
    style.Colors[ImGuiCol_TabUnfocusedActive] = _grey;
    style.Colors[ImGuiCol_DockingPreview] = _theme_clr;
    style.Colors[ImGuiCol_DockingEmptyBg] = _theme_clr;
    style.Colors[ImGuiCol_PlotLines] = _white;
    style.Colors[ImGuiCol_PlotLinesHovered] = _grey;
    style.Colors[ImGuiCol_PlotHistogram] = _white;
    style.Colors[ImGuiCol_PlotHistogramHovered] = _grey;
    style.Colors[ImGuiCol_TableHeaderBg] = _dark;
    style.Colors[ImGuiCol_TableBorderStrong] = _darkgrey;
    style.Colors[ImGuiCol_TableBorderLight] = _grey;
    style.Colors[ImGuiCol_TableRowBg] = _black;
    style.Colors[ImGuiCol_TableRowBgAlt] = _white;
    style.Colors[ImGuiCol_TextSelectedBg] = _darkgrey;
    style.Colors[ImGuiCol_DragDropTarget] = _darkgrey;
    style.Colors[ImGuiCol_NavHighlight] = _grey;
    style.Colors[ImGuiCol_NavWindowingHighlight] = _grey;
    style.Colors[ImGuiCol_NavWindowingDimBg] = _grey;
    style.Colors[ImGuiCol_ModalWindowDimBg] = _grey;
}

float bg_cols[] = { 0.15f, 0.15f, 0.15f };
float cube_cols[] = { 1.f, 1.f, 1.f };
float pos[] = { 0.f, 0.f, 0.f };
float rot[] = { 0.f, 0.f, 0.f };
void UI::update_pos(XEngine::Camera* t_camera) {
    glm::vec3 val = t_camera->position;
    pos[0] = val.x;
    pos[1] = val.y;
    pos[2] = val.z;
    rot[0] = t_camera->rotation.x;
    rot[1] = t_camera->rotation.y;
    rot[2] = t_camera->rotation.z;
}

static bool is_equal(float x, float y, float z, glm::vec3 t_a2) {
    return x == t_a2.x && y == t_a2.y && z == t_a2.z;
}

bool is_note_open = true;
bool is_editor_open = true;
bool is_scene_view_open = true;
bool is_file_viewer_open = true;
bool is_demo_win_open = false;
bool is_imgui_options_open = false;

static glm::vec3 DragFloat3(const char* t_name, glm::vec3 t_value,
    float t_add_val = 1.0f, float t_min = -1024.f, float t_max = 1024.f) {
    float conversion_array[3] = { t_value.x, t_value.y, t_value.z };
    ImGui::DragFloat3(t_name, conversion_array, t_add_val, t_min, t_max);
    return glm::vec3(conversion_array[0], conversion_array[1], conversion_array[2]);
}

static glm::vec3 ColorEdit3(const char* t_name, glm::vec3 t_value) {
    float conversion_array[3] = { t_value.x, t_value.y, t_value.z };
    ImGui::ColorEdit3(t_name, conversion_array);
    return glm::vec3(conversion_array[0], conversion_array[1], conversion_array[2]);
}

static glm::vec4 ColorEdit4(const char* t_name, glm::vec4 t_value) {
    float conversion_array[3] = { t_value.x, t_value.y, t_value.z };
    ImGui::ColorEdit3(t_name, conversion_array);
    return glm::vec4(conversion_array[0], conversion_array[1], conversion_array[2], 1.0f);
}

static glm::vec4 DragFloat4(const char* t_name, glm::vec4 t_value,
    float t_add_val = 1.0f, float t_min = -1024.f, float t_max = 1024.f) {
    float conversion_array[3] = { t_value.x, t_value.y, t_value.z };
    ImGui::DragFloat3(t_name, conversion_array, t_add_val, t_min, t_max);
    return glm::vec4(conversion_array[0], conversion_array[1], conversion_array[2], t_value.w);
}

Transform* scelected_obj = nullptr;
bool did_select = false;
static void draw_editor(XEngine::App* t_app, XEngine::Camera* t_camera) {
    //Basic values and info.
    ImGui::Text(("FPS: " + std::to_string(t_app->fps)).c_str());
    //Controls tab.
    if(ImGui::CollapsingHeader(u8"Управление")) {
        ImGui::TextColored(ImVec4(0.75f, 0.2f, 0.2f, 1.f), u8"Без ПКМ");
        ImGui::TextWrapped(u8"Shift + Колёсико мыши - Приближение/отдаление (FoV камеры)");
        ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.75f, 1.f), u8"С ПКМ");
        ImGui::TextWrapped(u8"Колёсико мыши - Движение вперёд/назад относительно камеры");
        ImGui::TextWrapped(u8"WASD - Схема движения");
        ImGui::TextWrapped(u8"Shift - Вниз");
        ImGui::TextWrapped(u8"Spacebar - Вверх");
        ImGui::TextColored(ImVec4(0.2f, 0.75f, 0.2f, 1.f), u8"Режимы отображения");
        ImGui::TextWrapped(u8"1 - Обычный");
        ImGui::TextWrapped(u8"2 - Wireframe");
        ImGui::TextWrapped(u8"3 - UV");
        ImGui::TextWrapped(u8"4 - Normal");
        ImGui::TextColored(ImVec4(0.75f, 0.5f, 0.2f, 1.f), u8"Дополнительно");
        ImGui::TextWrapped(u8"M - Проиграть тестовый звук");
        ImGui::TextWrapped(u8"T - Добавить силы движения к Rigidbody модели");
        ImGui::TextWrapped(u8"F - Режим фонарика");
        ImGui::TextWrapped(u8"R - Горячая перезагрузка (немного кушает память)");
    }
    //Object redactor.
    if(did_select) {
        if(ImGui::CollapsingHeader(scelected_obj->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            scelected_obj->position = DragFloat3("Position", scelected_obj->position, 0.1f);
            scelected_obj->rotation = DragFloat4("Rotation", scelected_obj->rotation, 0.1f);
            scelected_obj->size = DragFloat3("Scale", scelected_obj->size, 0.1f);
#ifdef XENGINE_IO
            if (ImGui::Button("Load model")) scelected_obj->load_model(XEngine::OS::open_file_dialog(scelected_obj->get_model_path()));
#endif
            ImGui::Text(("Instances: " + std::to_string(scelected_obj->instances_amount())).c_str());
            ImGui::Text(("Components (" + std::to_string(scelected_obj->components_amount()) + ")").c_str());
            //Components.
            {
                if(scelected_obj->get_component<LightSource>().is_initialized()) {
                    if(ImGui::TreeNode("LightSource")) {
                        ImGui::Text(("PointLight global ID: " + std::to_string(LightSource::global_id)).c_str());
                        scelected_obj->get_component<LightSource>().color = ColorEdit4("Color",
                            scelected_obj->get_component<LightSource>().color);
                        ImGui::TreePop();
                    }
                }
                if(scelected_obj->get_component<BoxCollider>().is_initialized()) {
                    if(ImGui::TreeNode("Box Collider")) {
                        scelected_obj->get_component<BoxCollider>().center =
                            DragFloat3("Center", scelected_obj->get_component<BoxCollider>().center, 0.1f);
                        scelected_obj->get_component<BoxCollider>().rotation =
                            DragFloat3("Rotation", scelected_obj->get_component<BoxCollider>().rotation, 0.1f);
                        scelected_obj->get_component<BoxCollider>().size =
                            DragFloat3("Size", scelected_obj->get_component<BoxCollider>().size, 0.1f);
                        ImGui::TreePop();
                    }
                }
                if(scelected_obj->get_component<SphereCollider>().is_initialized()) {
                    if(ImGui::TreeNode("Sphere Collider")) {
                        scelected_obj->get_component<SphereCollider>().center =
                            DragFloat3("Center", scelected_obj->get_component<SphereCollider>().center, 0.1f);
                        ImGui::DragFloat("Radius", &scelected_obj->get_component<SphereCollider>().radius, 0.1f);
                        ImGui::TreePop();
                    }
                }
                if(scelected_obj->get_component<Rigidbody>().is_initialized()) {
                    if(ImGui::TreeNode("Rigidbody")) {
                        ImGui::Checkbox("Use gravity", &scelected_obj->get_component<Rigidbody>().use_gravity);
                        float mass = scelected_obj->get_component<Rigidbody>().mass;
                        ImGui::DragFloat("Mass", &mass, 0.1f, 0.f);
                        scelected_obj->get_component<Rigidbody>().mass = mass;
                        scelected_obj->get_component<Rigidbody>().acceleration = DragFloat3("Acceleration",
                            scelected_obj->get_component<Rigidbody>().acceleration, 0.1f);
                        scelected_obj->get_component<Rigidbody>().velocity = DragFloat3("Velocity",
                            scelected_obj->get_component<Rigidbody>().velocity, 0.1f);
                        ImGui::TreePop();
                    }
                }
            }
            if(ImGui::Button("Delete")) {
                std::vector<Transform*>* vec = &Enviroment::get_current_scene()->transforms;
                auto it = std::find(vec->begin(), vec->end(), scelected_obj);
                did_select = false;
                scelected_obj = nullptr;
                if(it != vec->end()) {
                    (*it)->remove();
                    vec->erase(it);
                }
            }
        }
    } else if(ImGui::CollapsingHeader(u8"Среда")) {
        //Enviroment.
        XEngine::Enviroment::gravity = DragFloat3(u8"Гравитация", XEngine::Enviroment::gravity);
        //Window data.
        ImGui::Text(u8"Окно");
        bool vsync = t_app->window.get_param_b(XEngine::W_VSYNC);
        ImGui::Checkbox("V-Sync", &vsync);
        t_app->window.set_param(XEngine::W_VSYNC, vsync);
        XEngine::Renderer::set_clear_color(ColorEdit3("BG Color", XEngine::Renderer::get_clear_color()));
    }
    ImGui::End();
}

static void draw_scene_viewer(Scene* t_scene, Material* default_mat) {
    if(ImGui::Button("+"))
        SceneEditor::add_entry(SceneEditor::NTT_EMPTY, default_mat);
    ImGui::SameLine();
    if(ImGui::CollapsingHeader(t_scene->name)) {
        int i = 0;
        for(Transform* t : t_scene->transforms) {
            if(ImGui::Button((t->name + "##" + std::to_string(i)).c_str())) {
                scelected_obj = t;
                did_select = true;
            }
            i++;
        }
    } else did_select = false;
    ImGui::End();
}

std::string cur_path = fs::current_path().string() + "\\res";
static void draw_file_viewer() {
    ImGui::Text(cur_path.c_str());
    if(ImGui::Button("..")) cur_path = fs::directory_entry(cur_path).path().parent_path().string();
    ImGui::BeginTable("files", 1);
    for(const auto& entry : fs::directory_iterator(cur_path)) {
        if(entry.is_directory()) {
            if(ImGui::Button((u8"[D] " + entry.path().filename().u8string()).c_str())) cur_path = entry.path().string();
        }
        else ImGui::Button((u8"[F] " + entry.path().filename().u8string()).c_str());
        ImGui::TableNextColumn();
    }
    ImGui::EndTable();
    ImGui::End();
}

static void setup_dock(XEngine::App* t_app) {
    //Variables.
    bool open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;
    //Do fullscreen.
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("XEditor UI", &open, window_flags);
    ImGui::PopStyleVar(3);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGuiID dockspace_id = ImGui::GetID("XEditor UI");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    //Start menu.
    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("New Project (Ctrl+N)")) { }
            if(ImGui::MenuItem("Open Project (Ctrl+O)")) { }
            ImGui::Separator();
            if(ImGui::MenuItem("Save (Ctrl+S)")) { }
            if(ImGui::MenuItem("Save as... (Alt+S)")) {  }
            ImGui::Separator();
            if(ImGui::MenuItem("Exit (Esc)"))
                (*t_app).shutdown();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")) {
            if(ImGui::MenuItem("Hot reload (R)")) {}
            if(ImGui::MenuItem("Undo (Ctrl+Z)")) {}
            if(ImGui::MenuItem("Redo (Ctrl+Shift+Z)")) {}
            ImGui::Separator();
            if(ImGui::MenuItem("Project settings")) {}
            if(ImGui::MenuItem("Preferences")) {}
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View")) {
            ImGui::Text("Layouts");
            if(ImGui::MenuItem("Default")) {}
            if(ImGui::MenuItem("Wide")) {}
            if(ImGui::MenuItem("Tall")) {}
            ImGui::Separator();
            if(ImGui::BeginMenu("Windows")) {
                if(ImGui::MenuItem("Editor")) is_editor_open = true;
                if(ImGui::MenuItem("Files")) is_file_viewer_open = true;
                if(ImGui::MenuItem("Scene")) is_scene_view_open = true;
                if(ImGui::MenuItem("ImGui Demo")) is_demo_win_open = true;
                if(ImGui::MenuItem("ImGui Options")) is_imgui_options_open = true;
                if(ImGui::MenuItem("Update Note")) is_note_open = true;
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if(ImGui::MenuItem("Reset (Ctrl+R)")) ImGui::ClearIniSettings();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Build")) {
            if(ImGui::MenuItem("Compile project (Ctrl+B)")) {}
            if(ImGui::MenuItem("Build settings (Ctrl+Shift+B)")) {}
            if(ImGui::MenuItem("Delete last build (Alt+B)")) {}
            ImGui::Separator();
            if(ImGui::MenuItem("Publish... (Ctrl+P)")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Packages")) {
            if(ImGui::MenuItem("View connected (Ctrl+T)")) {}
            ImGui::Separator();
            if(ImGui::MenuItem("Add package... (Ctrl+Shift+T)")) {}
            if(ImGui::MenuItem("Create package... (Ctrl+G)")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

float font_size = 13.f;
const char* font_path = "res\\consola.ttf";
std::string new_font_path = "res\\consola.ttf";
bool auto_font_update = true;
void update_font_data(App* t_app) {
    font_size = std::clamp(font_size, 1.f, 128.f);
    t_app->window.ui_font_size = font_size;
    t_app->window.ui_font_dir = font_path;
    t_app->window.ui_need_to_reload = true;
}
void UI::draw(UIEditorData t_data) {
    //Setup.
    setup_dock(t_data.t_app);
    //Draw ImGui.
    if(is_demo_win_open) ImGui::ShowDemoWindow(&is_demo_win_open);
    if(is_editor_open) {
        ImGui::Begin("Editor", &is_editor_open);
        draw_editor(t_data.t_app, t_data.t_camera);
    }
    if(is_file_viewer_open) {
        ImGui::Begin("Files", &is_file_viewer_open);
        draw_file_viewer();
    }
    if(is_scene_view_open) {
        ImGui::Begin("Scene");
        draw_scene_viewer(t_data.t_scene, t_data.default_mat);
    }
    if(is_note_open) {
        ImGui::Begin("Note", &is_note_open);
        ImGui::TextWrapped(u8"Добро пожаловать в XEditor!");
        ImGui::TextWrapped(u8"  XEditor является GUI конфигурацией для программного слоя XEngine.");
        ImGui::Text("");
        ImGui::TextWrapped(u8"В этой версии:");
        ImGui::TextWrapped(u8"  * Улучшена система столкновений");
        ImGui::TextWrapped(u8"  * Улучшена система материалов");
        ImGui::TextWrapped(u8"  * Обновлён вид редактора");
        ImGui::TextWrapped(u8"  * Система пакетов");
        ImGui::TextWrapped(u8"  * Процедурная геометрия (плоскость и куб)");
        ImGui::TextWrapped(u8"  * Добавлен список изменений (это окно)");
        ImGui::Text("");
        ImGui::TextWrapped(u8"Удачных работ!");
        ImGui::Text("");
        ImGui::Text(u8"Версия: 0.1");
        ImGui::End();
    }
    if(is_imgui_options_open) {
        ImGui::Begin("ImGui Options", &is_imgui_options_open);
        ImGui::Text(u8"Текст");
        ImGui::DragFloat(u8"Размер", &font_size, 0.25f, 1.f, 128.f);
        ImGui::BeginDisabled(auto_font_update);
        ImGui::InputText(u8"Шрифт", &new_font_path);
        ImGui::EndDisabled();
        ImGui::Checkbox(u8"Авто перезагрузка", &auto_font_update);
        if(auto_font_update) update_font_data(t_data.t_app);
        else {
            if(ImGui::Button(u8"Перезагрузить")) {
                if(std::filesystem::exists(new_font_path)) font_path = new_font_path.c_str();
                update_font_data(t_data.t_app);
            }
        }
        ImGui::End();
    }
}