//
// Created by Emile Brückner on 1/13/26.
//

#include "../utils.hpp"
#include "../rendering/renderer.hpp"
#include "Window.hpp"

#include <iostream>
#include <GLFW/glfw3.h>
#include <sstream>

#include "../model/GraphSaver.hpp"



Window::Window() : renderer()
{
}

bool Window::initWindow()
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

    std::cout << "here";
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);


    gui.initGUI(window);



    // Query the framebuffer size, this can differ from the window size on some systems
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    // Create the renderer object
    Renderer renderer = Renderer(framebufferWidth, framebufferHeight);

	//Load GLAD so it configures OpenGL
	gladLoadGL();  //TODO: in renderer?

    renderer.init();


    // FPS counter TODO: Move to GUI, implement in ImGui
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
		glClearColor(0.f, 0.14f, 0.28f, 1.0f); //TODO: in renderer?
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT); //TODO: in renderer?

        glfwPollEvents();
        renderer.processEvents(window);


		//todo create a graphSaver in the propper place, after graph was imported, and give it as an attribute to renderer.runFrame
        // Draw frame from renderer



		GL_CHECK_ERROR();//TODO: in renderer?

		GraphSaver graphSaver;
        renderer.runFrame(graphSaver);
        GL_CHECK_ERROR();//TODO: in renderer?

		//load GUI
		gui.loadFrame();

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

	return true;
}
void Window::processEvents(GLFWwindow* m_window)
{
    // alternatively: use GLFW's glfwSetKeyCallback

    // F5 to reload shaders
    if (glfwGetKey(m_window, GLFW_KEY_F5) == GLFW_RELEASE)
    {
        mF5Pressed = false;
    } else
    {
        if (!mF5Pressed)
        {
            renderer.reloadShaders();
        }
        mF5Pressed = true;
    }

    // Camera Mouse TODO:
    static constexpr float PI = 3.1415926536f;
    static constexpr float mouse_radians_per_pixel = 0.003f;
    int right_mouse_state = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2);
    double mouse_position_double[2];
    glfwGetCursorPos(m_window, &mouse_position_double[0], &mouse_position_double[1]);
    float mouse_position[2] = {(float)mouse_position_double[0], (float)mouse_position_double[1]};
    if (!mCamera.rotate_camera && right_mouse_state == GLFW_PRESS) {
        mCamera.rotate_camera = true;
        mCamera.rotation_x_0 = mCamera.rotation_x + mouse_position[1] * mouse_radians_per_pixel;
        mCamera.rotation_y_0 = mCamera.rotation_y - mouse_position[0] * mouse_radians_per_pixel;
    }
    if (right_mouse_state == GLFW_RELEASE)
        mCamera.rotate_camera = false;
    if (mCamera.rotate_camera) {
        mCamera.rotation_x = mCamera.rotation_x_0 - mouse_radians_per_pixel * mouse_position[1];
        mCamera.rotation_y = mCamera.rotation_y_0 + mouse_radians_per_pixel * mouse_position[0];
        mCamera.rotation_x = (mCamera.rotation_x < -PI) ? -PI : mCamera.rotation_x;
        mCamera.rotation_x = (mCamera.rotation_x > PI) ? PI : mCamera.rotation_x;
    }
    static double last_time = 0.0;
    double now = glfwGetTime();
    double elapsed_time = (last_time == 0.0) ? 0.0 : (now - last_time);
    auto time_delta = (float)elapsed_time;
    last_time = now;
    float final_speed = mCamera.speed;
    final_speed *= (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? 10.0f : 1.0f;
    final_speed *= (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ? 0.1f : 1.0f;
    float step = time_delta * final_speed;

    // Camera Keyboard
    float forward = 0.0f, right = 0.0f, vertical = 0.0f;
    forward += (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) ? step : 0.0f;
    forward -= (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) ? step : 0.0f;
    right += (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) ? step : 0.0f;
    right -= (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) ? step : 0.0f;
    vertical += (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) ? step : 0.0f;
    vertical -= (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? step : 0.0f;
    float cos_y = cosf(mCamera.rotation_y), sin_y = sinf(mCamera.rotation_y);
    mCamera.position_world_space[0] +=  sin_y * forward;
    mCamera.position_world_space[0] +=  cos_y * right;
    mCamera.position_world_space[2] += -cos_y * forward;
    mCamera.position_world_space[2] +=  sin_y * right;
    mCamera.position_world_space[1] +=  vertical;
}

