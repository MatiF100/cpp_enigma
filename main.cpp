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

    Drum d2("E,K,M,F,L,G,D,Q,V,Z,N,T,O,W,Y,H,X,U,S,P,A,I,B,R,C,J", 0, 'Q' - 'A');
    Drum d1("A,J,D,K,S,I,R,U,X,B,L,H,W,T,M,C,Q,G,Z,N,P,Y,F,V,O,E", 0, 'E' - 'A');
    Drum d3("B,D,F,H,J,L,C,P,R,T,X,V,Z,N,Y,E,I,W,G,A,K,M,U,S,Q,O", 0, 'V' - 'A');
    Drum rev("Y,R,U,H,Q,S,L,D,P,X,N,G,O,K,M,I,E,B,F,Z,C,W,V,J,A,T", 0, 0);;

    std::string wiad = "aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa";
    std::cout << "Przed szyfrowaniem: " << wiad << std::endl << "Po szyfrowaniu: ";
    std::string szyfrogram;
    DrumAssembly module(d1, d2, d3, rev);
    module.set_drums_offset(1,0,0, 0);
    Machine wenigma;
    wenigma.set_wehrenigma(module);
    wenigma.detatch_drums();
    wenigma.set_wehrenigma(module);



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

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    static Plugboard* pboard = new Plugboard();
    bool show_demo_window = true;
    bool show_drums_window = false;
    bool show_plugboard_window = false;
    bool show_input_window = false;
    bool show_output_window = false;
    bool show_file_window = false;
    bool show_status_window = false;
    bool show_setup_window = false;
    std::string output = "Tu pojawi się twoja wiadomość";
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
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
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Drums Window", &show_drums_window);
            ImGui::Checkbox("Plugboard Window", &show_plugboard_window);
            ImGui::Checkbox("Input Window", &show_input_window);
            ImGui::Checkbox("Status Window", &show_status_window);
            ImGui::Checkbox("Setup Window", &show_setup_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Show drums offset window
        if (show_drums_window)
        {
            auto [r,m,l,lm,ref] = wenigma.get_offsets();
            int offsets[] = {r,m,l,lm,ref};
            const char* values[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25", "26"};
            ImGui::Begin("Drums panel", &show_drums_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Combo("Reflector drum", &offsets[4], values, IM_ARRAYSIZE(values));
            if(offsets[3]<26) {
                ImGui::Combo("Leftmost drum", &offsets[3], values, IM_ARRAYSIZE(values));
            }
            ImGui::Combo("Left drum", &offsets[2], values, IM_ARRAYSIZE(values));
            ImGui::Combo("Middle drum", &offsets[1], values, IM_ARRAYSIZE(values));
            ImGui::Combo("Right drum", &offsets[0], values, IM_ARRAYSIZE(values));
            ImGui::End();
            wenigma.set_drum_offsets(offsets[4], offsets[0], offsets[1], offsets[2], offsets[3]);
        }
        // Show plugboard configuration window
        if (show_plugboard_window){
            static bool pboard_attatched = false;
            if (wenigma.get_plugboard() != nullptr){
                delete pboard;
                pboard = wenigma.get_plugboard();
                pboard_attatched = true;
            }else{
                pboard_attatched = false;
            }
            static char lock;
            static ImVec4 colors[26];
            bool colored = false;
            ImVec4 base_color = (ImVec4)ImColor();
            ImGui::Begin("Plugboard", &show_plugboard_window);

            if(pboard_attatched){
                if(ImGui::Button("Detatch")){
                    pboard_attatched = false;
                    wenigma.detatch_plugboard();
                }
            }else{
                if(ImGui::Button("Attatch"))
                    pboard_attatched = true;
            }
            for(int i = 0; i<26; i++){

                colored = false;
                char tmp[2] = "A";
                tmp[0] += i;

                if(i!=0 && i%13!=0)
                    ImGui::SameLine();

                if(pboard->swap('A'+i) != 'A'+i){
                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, base_color);
                    colored = true;
                }

                if(ImGui::Button(tmp)){
                    if(pboard->remove_plug('A'+i)){
                        ImGui::PopStyleColor(1);
                        ImGui::PopID();
                        continue;
                    }
                    if (lock != 0){
                        bool result = pboard->insert_plug(lock, (char)('A'+i));
                        lock = 0;
                    }else{
                        lock = 'A'+i;
                    }
                }
                if(colored) {
                    ImGui::PopStyleColor(1);
                    ImGui::PopID();
                }
                if (pboard_attatched) wenigma.attatch_plugboard(*pboard);
            }
            ImGui::End();
        }
        //Show input message window
        if (show_input_window){
            static std::string msg;
            static ImGuiInputTextFlags flags_in = ImGuiInputTextFlags_AllowTabInput;
            static ImGuiInputTextFlags flags_out = ImGuiInputTextFlags_AllowTabInput;
            ImGui::Begin("Input", &show_input_window);

            ImGui::InputTextMultiline("##input", &msg, flags_in);
            ImGui::SameLine();
            if(ImGui::Button("Przetwórz!")){
                output = wenigma.process_message(msg);
            }

            flags_out |= ImGuiInputTextFlags_ReadOnly;
            ImGui::InputTextMultiline("##output", &output, flags_out);
            ImGui::End();
        }
        if (show_status_window){
            std::string variant;
            switch(wenigma.get_variant()){
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
            ImGui::End();

        }
        if(show_setup_window){
            static std::string ifile, ofile;
            ImGui::Begin("Configuration", &show_setup_window);
            ImGui::InputText("##iconfigfile", &ifile);
            ImGui::SameLine();
            if(ImGui::Button("Załaduj z pliku!")){
               if(wenigma.load_config(ifile));
            }

            ImGui::InputText("##oconfigfile", &ofile);
            ImGui::SameLine();
            if(ImGui::Button("Zapisz do pliku!")){
                if(wenigma.save_config(ofile));
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
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



    std::cout << "Hello, World!" << std::endl;



    szyfrogram = wenigma.process_message(wiad);
    std::cout << szyfrogram << std::endl << "Po deszyfracji: ";

    wenigma.set_drum_offsets(0, 0, 0, 1);
    szyfrogram = wenigma.process_message(szyfrogram);
    std::cout << szyfrogram << std::endl ;

    return 0;
}
