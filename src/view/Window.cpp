//
// Created by Emile Brückner on 1/13/26.
//

#include "../utils.hpp"
#include "../rendering/renderer.hpp"
#include "Window.hpp"

#include "imgui/imgui.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include <sstream>

#include "controller/ButtonController.hpp"
#include "controller/ErrorCollector.hpp"


namespace graphvise {
    bool Window::initWindow() {
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

        const auto defaultWidth = currentRes.width;
        const auto defaultHeight = currentRes.height;

        window = glfwCreateWindow(defaultWidth, defaultHeight, windowTitle.c_str(), nullptr, nullptr);
        // Error check if the window fails to create

        glfwSetWindowSizeLimits(window, 0, 640, GLFW_DONT_CARE, GLFW_DONT_CARE);

        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

		// Introduce the window into the current context
		glfwMakeContextCurrent(window);

        //Load GLAD so it configures OpenGL
        gladLoadGL();

        // enable depth testing for rendering
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // TEMPORARY: Disable depth test
        //glDisable(GL_DEPTH_TEST);

        // Initialize ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();


        // Query the framebuffer size, this can differ from the window size on some systems
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    	// set welcome graph
    	GraphSaver::getInstance().setGraph(WelcomeGraph());
    	renderer->m_camera().position_world_space = glm::vec3(0, 0, 15);



        // Create the renderer object
        renderer->resize(framebufferWidth, framebufferHeight);

        renderer->init();

        ErrorCollector::getInstance().signIn(gui);

        gui.initGUI(window);

    	inputManager.initInputManager(window, &gui, buttonController);

    	return true;

    }

    void Window::startApplicationLoop()
    {

    	// FPS counter
    	int frameCount = 0;
    	double accumulatedTime = 0.0;

    	// Main while loop
    	while (!glfwWindowShouldClose(window))
    	{
    		const double startTime = glfwGetTime();

    		// Resize the renderer and viewport if the framebuffer / window size changed
    		int newFramebufferWidth, newFramebufferHeight;
    		glfwGetFramebufferSize(window, &newFramebufferWidth, &newFramebufferHeight);
    		if (newFramebufferWidth != framebufferWidth || newFramebufferHeight != framebufferHeight)
    		{
    			framebufferWidth = newFramebufferWidth;
    			framebufferHeight = newFramebufferHeight;
    			glViewport(0, 0, framebufferWidth, framebufferHeight);
    			renderer->resize(framebufferWidth, framebufferHeight);
    		}


    		// Draw frame from renderer
    		GL_CHECK_ERROR();
    		renderer->runFrame();
    		GL_CHECK_ERROR();

    		//load GUI
    		gui.loadFrame(framebufferWidth, framebufferHeight);

    		inputManager.processInput();


    		// Swap the back buffer with the front buffer
    		glfwSwapBuffers(window);


    		// FPS counter
    		const double endTime = glfwGetTime();
    		accumulatedTime += endTime - startTime;
    		++frameCount;
    		if (1.0 < accumulatedTime)
    		{
    			assert(0 < frameCount);

    			gui.setFps(frameCount / accumulatedTime);

    			accumulatedTime = 0.0;
    			frameCount = 0;
    		}
    	}

    	CachingController::cacheCurrentGraph();

    	gui.shutdownGUI();

    	// Renderer cleanup
    	renderer->shutdown();

    	// Delete window before ending the program
    	glfwDestroyWindow(window);

    	// Terminate GLFW before ending the program
    	glfwTerminate();

    }
		Graph Window::WelcomeGraph(){

       return Graph({
                                       {-12, 3, 0},
                                       {-11, 0, 0},
                                       {-10, 1, 0},
                                       {-9, 0, 0},
                                       {-8, 3, 0},
                                       {-7, 3, 0},
                                       {-5, 3, 0},
                                       {-7, 1.5, 0},
                                       {-6, 1.5, 0},
                                       {-7, 0, 0},
                                       {-5, 0, 0},
                                       {-4, 3, 0},
                                       {-4, 0, 0},
                                       {-2, 0, 0},
                                       {-1, 3, 0},
                                       {1, 3, 0},
                                       {-1, 0, 0},
                                       {1, 0, 0},
                                       {2, 3, 0},
                                       {4, 3, 0},
                                       {2, 0, 0},
                                       {4, 0, 0},
                                       {5, 0, 0},
                                       {6, 3, 0},
                                       {7, 2, 0},
                                       {8, 3, 0},
                                       {9, 0, 0},
                                       {10, 3, 0},
                                       {12, 3, 0},
                                       {10, 1.5, 0},
                                       {11, 1.5, 0},
                                       {10, 0, 0},
                                       {12, 0, 0}

                                   },
                                   {
                                       {0, 1},
                                       {1, 2},
                                       {2, 3},
                                       {3, 4},
                                       {5, 6},
                                       {5,7},
                                       {7, 8},
                                       {7, 9},
                                       {9, 10},
                                       {11, 12},
                                       {12, 13},
                                       {14, 15},
                                       {14, 16},
                                       {16, 17},
                                       {18, 19},
                                       {18, 20},
                                       {19, 21},
                                       {20, 21},
                                       {22, 23},
                                       {23,24},
                                       {24,25},
                                       {25,26},
                                       {27,28},
                                       {27,29},
                                       {29,30},
                                       {29,31},
                                       {31,32}
                                   },

                                   "Welcome to GraphVise");


    }

}