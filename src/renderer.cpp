#include "renderer.hpp"
#include "utils.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

Renderer::Renderer(int framebufferWidth, int framebufferHeight)
        : mColor{1.f, 0.55f, 0.f, 1.0f}, mShaderProgram(0),
        mVertexShaderPath(std::string(SHADERS_PATH) + std::string("/simple.vert")),
        mFragmentShaderPath(std::string(SHADERS_PATH) + std::string("/single_color.frag")),
        mVAO(0), mVBO(0), mCamera(), mFramebufferSize(framebufferWidth, framebufferHeight), mF5Pressed(false)
{
}

/**
 * Initialize the renderer and all of its (OpenGL) ressources. Must be called before runFrame().
 */
void Renderer::init()
{
    // Load the shader files
    reloadShaders();

    // Vertex Data -----------------------------------------------------------------------------------------------------
    // Generate the VAO and VBO with only 1 object each
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    // Make the VAO the current vertex array object by binding it
    glBindVertexArray(mVAO);

    // Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Configure the vertex attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GL_CHECK_ERROR();
}

/**
 * Reloads the shaders from the file paths and compiles a new shader program to use.
 */
void Renderer::reloadShaders()
{
    // Create shader program object and get its reference
    GLuint newProgram = utils::createShaderProgramFromFile(mVertexShaderPath, mFragmentShaderPath);
    if (newProgram != 0)
    {
        if (mShaderProgram != 0)
            glDeleteProgram(mShaderProgram);
        mShaderProgram = newProgram;
    }
    GL_CHECK_ERROR();
}

/**
 * Called in the main loop to render a new frame.
 */
void Renderer::runFrame()
{
    if(mShaderProgram == 0)
    {
        std::cerr << "No shader program!" << std::endl;
        return;
    }

    // Tell OpenGL which shader program we want to use
    glUseProgram(mShaderProgram);
    GL_CHECK_ERROR();

    // Set program uniforms
    glUseProgram(mShaderProgram);
    glUniform4f(glGetUniformLocation(mShaderProgram, "color"), mColor[0], mColor[1], mColor[2], mColor[3]);
    glm::mat4 mvp = mCamera.get_world_to_projection_space(getAspectRatio());
    glUniformMatrix4fv(glGetUniformLocation(mShaderProgram, "mvp"), 1, false, &mvp[0][0]);
    GL_CHECK_ERROR();

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    GL_CHECK_ERROR();
}

/**
 * Frees and deletes all acquired (OpenGL) objects. Objects are freed in inverse order of how they were acquired.
 */
void Renderer::shutdown()
{
    // delete all the objects we've created
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteProgram(mShaderProgram);
}


void Renderer::processEvents(GLFWwindow* m_window)
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
            reloadShaders();
        }
        mF5Pressed = true;
    }

    // Camera Mouse
    static constexpr float PI = 3.1415926536f;
    static const float mouse_radians_per_pixel = 0.003f;
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
    float time_delta = (float)elapsed_time;
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
    vertical += (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) ? step : 0.0f;
    vertical -= (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) ? step : 0.0f;
    float cos_y = cosf(mCamera.rotation_y), sin_y = sinf(mCamera.rotation_y);
    mCamera.position_world_space[0] +=  sin_y * forward;
    mCamera.position_world_space[0] +=  cos_y * right;
    mCamera.position_world_space[2] += -cos_y * forward;
    mCamera.position_world_space[2] +=  sin_y * right;
    mCamera.position_world_space[1] +=  vertical;
}

void Renderer::resize(int framebufferWidth, int framebufferHeight)
{
    mFramebufferSize.x = framebufferWidth;
    mFramebufferSize.y = framebufferHeight;
}

float Renderer::getAspectRatio() const {
    return static_cast<float>(mFramebufferSize.x) / static_cast<float>(mFramebufferSize.y);
}
