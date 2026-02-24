//
// Created by Emile Brückner on 2/24/26.
//

#include "InputManager.hpp"
namespace graphvise
{
	double InputManager::scrollYOffset = 0;

	void InputManager::initInputManager(GLFWwindow* window)
	{
		this->window = window;

		glfwSetScrollCallback(this->window, scrollCallback);

	}

	void InputManager::processInput()
	{

		// Take care of all GLFW events

		glfwPollEvents();


		bool sprinting = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);


		if (!ImGui::GetIO().WantCaptureKeyboard)
		{

			//Moving Camera
			glm::vec3 direction(0, 0, 0);
			direction.z += (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.z -= (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.x += (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.x -= (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.y += (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) ? 1.0f : 0.0f;
			direction.y -= (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) ? 1.0f : 0.0f;
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

		if (!ImGui::GetIO().WantCaptureMouse){
			movementController.zoom(-scrollYOffset, sprinting);
			scrollYOffset = 0;
		}

		processHotkeys();
	}

	void InputManager::processHotkeys()
	{
		for (auto& [keys, function, pressedInPrevFrame] : hotkeys)
		{
			bool allKeysPressed = true;

			for (const int key : keys)
			{

				if (glfwGetKey(window, key) != GLFW_PRESS)
				{
					allKeysPressed = false;
					break;
				}

			}
			if (allKeysPressed && !pressedInPrevFrame)
			{
				function();
				pressedInPrevFrame = true;
			}
			else if (!allKeysPressed)
			{
				pressedInPrevFrame = false;
			}

		}


	}

	void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		scrollYOffset = yoffset;
	}
}