#include "Input.h"

Input::Input(Player* player)
    : m_Player(player), m_TogglePolygonMode(false)
{

}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Camera* cam = m_Player->GetCam();

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN ||
            key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT)
            cam->Rotate(key);
        else if (key == GLFW_KEY_W || key == GLFW_KEY_A ||
            key == GLFW_KEY_S || key == GLFW_KEY_D)
            cam->Move(key);
        else if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        else if (key == GLFW_KEY_P)
        {
            if (m_TogglePolygonMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    m_Player->UpdateChunkGridPosition();
}

void Input::TogglePolygonMode()
{
    m_TogglePolygonMode = !m_TogglePolygonMode;
    
    m_TogglePolygonMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Player* Input::GetPlayer()
{
    return m_Player;
}