#include <iostream>
#include "Drum.h"
#include "DrumAssembly.h"
#include "DrumAssemblyK.h"

#include "Machine.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_stdlib.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ENIGMA", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    std::fstream dummy;
    dummy.open("lato.ttf", std::ios_base::in | std::ios_base::binary);
    if (dummy.good()) {
        dummy.close();
        ImVector<ImWchar> ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddText("ęóąśłżźćńĘÓĄŚŁŻŹŃĆ");
        builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
        builder.BuildRanges(&ranges);

        io.Fonts->AddFontFromFileTTF("lato.ttf", 16, NULL, ranges.Data);
        io.Fonts->Build();
    }



    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    static Plugboard* pboard = new Plugboard();
    bool show_demo_window = false;
    bool show_drums_window = false;
    bool show_plugboard_window = false;
    bool show_input_window = false;
    bool show_output_window = false;
    bool show_selection_window = false;
    bool show_status_window = false;
    bool show_setup_window = false;
    std::string output = "Tu pojawi się twoja wiadomość";
    Machine* enigma = nullptr;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {

            ImGui::Begin("Module selection");

            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Checkbox("Drums Window", &show_drums_window);
            ImGui::Checkbox("Plugboard Window", &show_plugboard_window);
            ImGui::Checkbox("Input Window", &show_input_window);
            ImGui::Checkbox("Status Window", &show_status_window);
            ImGui::Checkbox("Setup Window", &show_setup_window);


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        if (enigma == nullptr) {
            static std::fstream drums;
            static std::vector<Drum> standard_drums;
            static std::vector<Drum> reflectors;
            static std::vector<Drum> thin_drums;
            static std::vector<Drum> thin_reflectors;
            static int variant;
            static int indexes[5]{};
            static int ring_offsets[5]{};
            ImGui::Begin("Assembly");
            if(!drums.is_open() && standard_drums.empty() && reflectors.empty() && thin_drums.empty() && thin_reflectors.empty()){
                drums.open("drums.cfg", std::ios_base::in);
            }else if (standard_drums.empty() && reflectors.empty() && thin_drums.empty() && thin_reflectors.empty()){
                Drum tmp;
                while(drums >> tmp){
                    if(tmp.narrow == true && tmp.reflector == false)
                        thin_drums.push_back(tmp);
                    else if(tmp.narrow == true && tmp.reflector == true)
                        thin_reflectors.push_back(tmp);
                    else if(tmp.narrow == false && tmp.reflector == true)
                        reflectors.push_back(tmp);
                    else standard_drums.push_back(tmp);
                }
                drums.close();
            }
            else if(variant == 0){
                ImGui::Text("Please select which configuration would you like to emulate...");
                ImGui::RadioButton("Wehrmacht confifuration", &variant, 1);
                ImGui::RadioButton("Kriegsmarine configuration", &variant, 2);
            }
            else if (!standard_drums.empty() && !reflectors.empty() && !thin_drums.empty() && !thin_reflectors.empty() && variant == 2) {
                ImGui::Text("Emulation mode:");
                ImGui::RadioButton("Wehrmacht confifuration", &variant, 1);
                ImGui::RadioButton("Kriegsmarine configuration", &variant, 2);

                const char *values[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
                                        "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26"};

                ImGui::Text("First column is for setting ring offset of a drum");
                ImGui::Text("Second column is for selection of the acual drum by it's name");
                ImGui::PushItemWidth(80);

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##rok1", &ring_offsets[0], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if (ImGui::BeginCombo("<-- Right drum", standard_drums[indexes[0]].name.c_str())) {
                    for (int n = 0; n < standard_drums.size(); n++) {
                        const bool is_selected = (indexes[0] == n);
                        if (ImGui::Selectable(standard_drums[n].name.c_str(), is_selected))
                            indexes[0] = n;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##rok2", &ring_offsets[1], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if (ImGui::BeginCombo("<-- Middle drum", standard_drums[indexes[1]].name.c_str())) {
                    for (int n = 0; n < standard_drums.size(); n++) {
                        const bool is_selected = (indexes[1] == n);
                        if (ImGui::Selectable(standard_drums[n].name.c_str(), is_selected))
                            indexes[1] = n;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##rok3", &ring_offsets[2], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if (ImGui::BeginCombo("<-- Left drum", standard_drums[indexes[2]].name.c_str())) {
                    for (int n = 0; n < standard_drums.size(); n++) {
                        const bool is_selected = (indexes[2] == n);
                        if (ImGui::Selectable(standard_drums[n].name.c_str(), is_selected))
                            indexes[2] = n;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##rok4", &ring_offsets[3], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if (ImGui::BeginCombo("<-- Thin, leftmost drum", thin_drums[indexes[3]].name.c_str())) {
                    for (int n = 0; n < thin_drums.size(); n++) {
                        const bool is_selected = (indexes[3] == n);
                        if (ImGui::Selectable(thin_drums[n].name.c_str(), is_selected))
                            indexes[3] = n;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##rok5", &ring_offsets[4], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if (ImGui::BeginCombo("<-- Reflector", thin_reflectors[indexes[4]].name.c_str())) {
                    for (int n = 0; n < thin_reflectors.size(); n++) {
                        const bool is_selected = (indexes[4] == n);
                        if (ImGui::Selectable(thin_reflectors[n].name.c_str(), is_selected))
                            indexes[4] = n;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopItemWidth();
                if (ImGui::Button("Set configuration")) {

                    Drum& r = standard_drums[indexes[0]];
                    Drum& m = standard_drums[indexes[1]];
                    Drum& l = standard_drums[indexes[2]];
                    Drum& lm = thin_drums[indexes[3]];
                    Drum& ref = thin_reflectors[indexes[4]];

                    r.set_ring_offset(ring_offsets[0]);
                    m.set_ring_offset(ring_offsets[1]);
                    l.set_ring_offset(ring_offsets[2]);
                    lm.set_ring_offset(ring_offsets[3]);
                    ref.set_ring_offset(ring_offsets[4]);

                    DrumAssemblyK assembly_module(r,m,l,lm,ref);
                    enigma = new Machine(assembly_module);
                    standard_drums.clear();
                    reflectors.clear();
                    thin_reflectors.clear();
                    thin_drums.clear();
                }


            }
            else if (!standard_drums.empty() && !reflectors.empty()  && variant == 1){
                ImGui::Text("Emulation mode:");
                ImGui::RadioButton("Wehrmacht confifuration", &variant, 1);
                ImGui::RadioButton("Kriegsmarine configuration", &variant, 2);

                const char *values[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
                                        "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26"};

                ImGui::Text("First column is for setting ring offset of a drum");
                ImGui::Text("Second column is for selection of the acual drum by it's name");

                ImGui::PushItemWidth(80);

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##ro1", &ring_offsets[0], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if(ImGui::BeginCombo("<-- Right drum", standard_drums[indexes[0]].name.c_str())){
                    for (int n = 0; n<standard_drums.size(); n++){
                        const bool is_selected = (indexes[0] == n);
                        if(ImGui::Selectable(standard_drums[n].name.c_str(), is_selected))
                            indexes[0] = n;

                        if(is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##ro2", &ring_offsets[1], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if(ImGui::BeginCombo("<-- Middle drum", standard_drums[indexes[1]].name.c_str())){
                    for (int n = 0; n<standard_drums.size(); n++){
                        const bool is_selected = (indexes[1] == n);
                        if(ImGui::Selectable(standard_drums[n].name.c_str(), is_selected))
                            indexes[1] = n;

                        if(is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##ro3", &ring_offsets[2], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if(ImGui::BeginCombo("<-- Left drum", standard_drums[indexes[2]].name.c_str())){
                    for (int n = 0; n<standard_drums.size(); n++){
                        const bool is_selected = (indexes[2] == n);
                        if(ImGui::Selectable(standard_drums[n].name.c_str(), is_selected))
                            indexes[2] = n;

                        if(is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::SetNextItemWidth(50);
                ImGui::Combo("##ro4", &ring_offsets[4], values, IM_ARRAYSIZE(values));
                ImGui::SameLine();

                if(ImGui::BeginCombo("<-- Reflector", reflectors[indexes[4]].name.c_str())){
                    for (int n = 0; n<reflectors.size(); n++){
                        const bool is_selected = (indexes[4] == n);
                        if(ImGui::Selectable(reflectors[n].name.c_str(), is_selected))
                            indexes[4] = n;

                        if(is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::PopItemWidth();

                if(ImGui::Button("Set configuration")){
                    Drum& r = standard_drums[indexes[0]];
                    Drum& m = standard_drums[indexes[1]];
                    Drum& l = standard_drums[indexes[2]];
                    Drum& ref = reflectors[indexes[4]];

                    r.set_ring_offset(ring_offsets[0]);
                    m.set_ring_offset(ring_offsets[1]);
                    l.set_ring_offset(ring_offsets[2]);
                    ref.set_ring_offset(ring_offsets[4]);

                    DrumAssembly assembly_module(r,m,l,ref);

                    enigma = new Machine(assembly_module);
                    standard_drums.clear();
                    reflectors.clear();
                    thin_reflectors.clear();
                    thin_drums.clear();
                }


            }else{
                standard_drums.clear();
                reflectors.clear();
                thin_reflectors.clear();
                thin_drums.clear();
            }
            ImGui::End();



        } else {
            // Show drums offset window
            if (show_drums_window) {
                auto[r, m, l, lm, ref] = enigma->get_offsets();
                int offsets[] = {r, m, l, lm, ref};
                const char *values[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
                                        "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26"};
                ImGui::Begin("Drums panel",
                             &show_drums_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Combo("Reflector drum", &offsets[4], values, IM_ARRAYSIZE(values));
                if (offsets[3] < 26) {
                    ImGui::Combo("Leftmost drum", &offsets[3], values, IM_ARRAYSIZE(values));
                }
                ImGui::Combo("Left drum", &offsets[2], values, IM_ARRAYSIZE(values));
                ImGui::Combo("Middle drum", &offsets[1], values, IM_ARRAYSIZE(values));
                ImGui::Combo("Right drum", &offsets[0], values, IM_ARRAYSIZE(values));
                ImGui::End();
                enigma->set_drum_offsets(offsets[4], offsets[0], offsets[1], offsets[2], offsets[3]);
            }
            // Show plugboard configuration window
            if (show_plugboard_window) {
                static bool pboard_attatched = false;
                if (enigma->get_plugboard() != nullptr) {
                    delete pboard;
                    pboard = enigma->get_plugboard();
                    pboard_attatched = true;
                } else {
                    pboard_attatched = false;
                }
                static char lock;
                static ImVec4 colors[26];
                bool colored = false;
                ImVec4 base_color = (ImVec4) ImColor();
                ImGui::Begin("Plugboard", &show_plugboard_window);

                if (pboard_attatched) {
                    if (ImGui::Button("Detatch")) {
                        pboard_attatched = false;
                        enigma->detatch_plugboard();
                    }
                } else {
                    if (ImGui::Button("Attatch"))
                        pboard_attatched = true;
                }
                for (int i = 0; i < 26; i++) {

                    colored = false;
                    char tmp[2] = "A";
                    tmp[0] += i;

                    if (i != 0 && i % 13 != 0)
                        ImGui::SameLine();

                    if (pboard->swap('A' + i) != 'A' + i) {
                        ImGui::PushID(i);
                        ImGui::PushStyleColor(ImGuiCol_Button, base_color);
                        colored = true;
                    }

                    if (ImGui::Button(tmp)) {
                        if (pboard->remove_plug('A' + i)) {
                            ImGui::PopStyleColor(1);
                            ImGui::PopID();
                            continue;
                        }
                        if (lock != 0) {
                            bool result = pboard->insert_plug(lock, (char) ('A' + i));
                            lock = 0;
                        } else {
                            lock = 'A' + i;
                        }
                    }
                    if (colored) {
                        ImGui::PopStyleColor(1);
                        ImGui::PopID();
                    }
                    if (pboard_attatched) enigma->attatch_plugboard(*pboard);
                }
                ImGui::End();
            }
            //Show input message window
            if (show_input_window) {
                static std::string msg;
                static ImGuiInputTextFlags flags_in = ImGuiInputTextFlags_AllowTabInput;
                static ImGuiInputTextFlags flags_out = ImGuiInputTextFlags_AllowTabInput;
                ImGui::Begin("Input", &show_input_window);

                ImGui::InputTextMultiline("##input", &msg, flags_in);
                ImGui::SameLine();
                if (ImGui::Button("Przetwórz!")) {
                    output = enigma->process_message(msg);
                }

                flags_out |= ImGuiInputTextFlags_ReadOnly;
                ImGui::InputTextMultiline("##output", &output, flags_out);
                ImGui::End();
            }
            if (show_status_window) {
                std::string variant;
                switch (enigma->get_variant()) {
                    case 1:
                        variant = "WEHR-ENIGMA";
                        break;
                    case 2:
                        variant = "KRIEGS-ENIGMA";
                        break;
                    default:
                        variant = "NONE";
                        break;
                }
                ImGui::Begin("Status", &show_status_window);
                ImGui::Text("Current variant: %s", variant.c_str());
                if(ImGui::Button("Disassemble")){
                    delete enigma;
                    enigma = nullptr;
                }
                ImGui::End();

            }
            if (show_setup_window) {
                static std::string ifile, ofile;
                ImGui::Begin("Configuration", &show_setup_window);
                ImGui::InputText("##iconfigfile", &ifile);
                ImGui::SameLine();
                if (ImGui::Button("Załaduj z pliku!")) {
                    if (enigma->load_config(ifile));
                }

                ImGui::InputText("##oconfigfile", &ofile);
                ImGui::SameLine();
                if (ImGui::Button("Zapisz do pliku!")) {
                    if (enigma->save_config(ofile));
                }
                ImGui::End();
            }

        }
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}
