//
// Created by Emile Brückner on 1/13/26.
//

#include "../utils.hpp"
#include "../rendering/renderer.hpp"
#include "Window.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <sstream>


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
    std::string windowTitle = "GraphVise";


	int width=1280, height=720;

	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, NULL);
	// Error check if the window fails to create
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);


    gui.initGUI(window);

    //Load GLAD so it configures OpenGL
    gladLoadGL();


    // Query the framebuffer size, this can differ from the window size on some systems
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    // Create the renderer object
    auto renderer = Renderer(framebufferWidth, framebufferHeight);
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


        // Draw frame from renderer
        renderer.runFrame();

		//load GUI
        gui.loadFrame();

		GL_CHECK_ERROR();


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

	gui.shutdownGUI();

    // Renderer cleanup
    renderer.shutdown();

    // Delete window before ending the program
    glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
