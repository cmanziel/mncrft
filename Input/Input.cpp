#include "Input.h"

Input::Input(Player* player)
    : m_Player(player), togglePolygonMode(false)
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
            togglePolygonMode = !togglePolygonMode;
    }

    m_Player->UpdateChunkGridPosition();
}

Player* Input::GetPlayer()
{
    return m_Player;
}