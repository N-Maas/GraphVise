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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "rendering/stb_image_write.h"

namespace graphvise {

	Window::Window()
	= default;
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

		int defaultWidth = currentRes.width;
		int defaultHeight = currentRes.height;

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
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();


		// Query the framebuffer size, this can differ from the window size on some systems
		int framebufferWidth, framebufferHeight;
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

		// Create the renderer object
		std::shared_ptr<Renderer> renderer = Renderer::getInstance(framebufferWidth, framebufferHeight);
		renderer->init();

		ButtonController controller = ButtonController(*renderer);

		std::shared_ptr<GUI> gui = std::make_shared<GUI>(&controller);

		ErrorCollector::getInstance().signIn(gui);


		gui->initGUI(window);

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
				renderer->resize(framebufferWidth, framebufferHeight);
			}

			// Specify the color of the background
			glClearColor(0.f, 0.14f, 0.28f, 1.0f);
			// Clean the back buffer and assign the new color to it
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwPollEvents();
			renderer->processEvents(window);

			processEvents();

			// Draw frame from renderer
			GL_CHECK_ERROR();
			renderer->runFrame();
			GL_CHECK_ERROR();



			//load GUI
			gui->loadFrame(framebufferWidth, framebufferHeight);

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

				gui->setFps(frameCount / accumulatedTime);

				accumulatedTime = 0.0;
				frameCount = 0;
			}
		}

		gui->shutdownGUI();

		// Renderer cleanup
		renderer->shutdown();

		// Delete window before ending the program
		glfwDestroyWindow(window);

		// Terminate GLFW before ending the program
		glfwTerminate();
		return true;
	}

	void Window::processEvents()
	{
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{

			(glfwGetKey(window, GLFW_KEY_F5) == GLFW_RELEASE); // for reloading shaders

			//Moving Camera
			glm::vec3 direction(0, 0, 0);
			direction.z += (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.z -= (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.x += (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.x -= (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.y += (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.y -= (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) ? 1.0f : 0.0f;
			bool sprinting = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
			movementController.moveCamera(direction, sprinting);

		}
		//Rotating Camera
		static float lastMousePosition[2];
		static double currentMousePositionDouble[2];
		glfwGetCursorPos(window, &currentMousePositionDouble[0], &currentMousePositionDouble[1]);
		float currentMousePositionFloat[2] = { static_cast<float>(currentMousePositionDouble[0]), static_cast<float>(currentMousePositionDouble[1]) };

		static bool rotatingCamera = false;
		int rightMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
		if (!rotatingCamera && rightMouseState == GLFW_PRESS) {
			rotatingCamera = true;
			std::ranges::copy(currentMousePositionFloat, std::begin(lastMousePosition));
		}
		if (rotatingCamera) {
			float yawChange = currentMousePositionFloat[0] - lastMousePosition[0];
			float pitchChange = lastMousePosition[1] - currentMousePositionFloat[1];
			movementController.rotateCamera(pitchChange, yawChange);
			std::ranges::copy(currentMousePositionFloat, std::begin(lastMousePosition));
		}
		if (rotatingCamera && rightMouseState == GLFW_RELEASE) {
			rotatingCamera = false;
		}
	}
}
