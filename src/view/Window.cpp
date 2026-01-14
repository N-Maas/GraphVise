//
// Created by Emile Brückner on 1/13/26.
//

#include "Window.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

#include "../utils.hpp"
#include "../rendering/renderer.hpp"



Window::Window()
{

}

int Window::initWindow()
{
    // If OpenMP is installed we can use it for parallelization
    utils::printOpenMPVersion();

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window
    std::string windowTitle = "Thesis Framework";
	GLFWwindow* window = glfwCreateWindow(800, 800, windowTitle.c_str(), nullptr, NULL);
	// Error check if the window fails to create
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

    // Query the framebuffer size, this can differ from the window size on some systems
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    // Create the renderer object
    Renderer renderer = Renderer(framebufferWidth, framebufferHeight);
    renderer.init();

    // FPS counter
    int frameCount = 0;
    double accumulatedTime = 0.0;

    // Main while loop
	while (!glfwWindowShouldClose(window))
	{
        double startTime = glfwGetTime();

        // Resize the renderer and viewport if the framebuffer / window size changed
        int newFramebufferWidth, newFramebufferHeight;
        glfwGetFramebufferSize(window, &newFramebufferWidth, &newFramebufferHeight);
        if (newFramebufferWidth != framebufferWidth || newFramebufferHeight != framebufferHeight)
        {
            framebufferWidth = newFramebufferWidth;
            framebufferHeight = newFramebufferHeight;
            glViewport(0, 0, framebufferWidth, framebufferHeight);
            renderer.resize(framebufferWidth, framebufferHeight);
        }

		// Specify the color of the background
		glClearColor(0.f, 0.14f, 0.28f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        renderer.processEvents(window);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGUI elements
		ImGui::Begin("CG Thesis Framework GUI");
		ImGui::Text("Control the Camera with WASD+QE + right mouse, hit F5 to reload shaders.");
		ImGui::Text("Add your GUI elements here!");
		ImGui::ColorEdit4("Color", &renderer.mColor.r);
		ImGui::End();

		//todo create a graphSaver in the propper place, after graph was imported, and give it as an attribute to renderer.runFrame
        // Draw frame from renderer
        renderer.runFrame(graphSaver);
        GL_CHECK_ERROR();

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();


        // FPS counter
        double endTime = glfwGetTime();
        accumulatedTime += endTime - startTime;
        ++frameCount;
        if (1.0 < accumulatedTime) {
            assert(0 < frameCount);
            std::ostringstream oss;
            oss << windowTitle << " - " << frameCount / accumulatedTime << " fps";
            glfwSetWindowTitle(window, oss.str().c_str());
            accumulatedTime = 0.0;
            frameCount = 0;
        }
	}

    // Deletes all ImGUI instances
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Renderer cleanup
    renderer.shutdown();

    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
