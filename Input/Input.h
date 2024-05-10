#pragma once

#include "../Player/Player.h"
#include <GLFW/glfw3.h>

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Input {
public:
    Input(Player* player);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    Player* GetPlayer();
    //void CheckKeyInput();

    void TogglePolygonMode();

private:
    Player* m_Player;
    bool m_TogglePolygonMode;
};

//void Input::CheckKeyInput() {
// 
//    Camera* cam = m_Player->GetCam();
// 
//    if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
//        m_Camera->Rotate(GLFW_KEY_UP);
//    if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
//        m_Camera->Rotate(GLFW_KEY_DOWN);
//    if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
//        m_Camera->Rotate(GLFW_KEY_LEFT);
//    if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//        m_Camera->Rotate(GLFW_KEY_RIGHT);
//
//    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
//        m_Camera->Move(GLFW_KEY_W);
//    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
//        m_Camera->Move(GLFW_KEY_A);
//    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
//        m_Camera->Move(GLFW_KEY_S);
//    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
//        m_Camera->Move(GLFW_KEY_D);
//
//    if (glfwGetKey(m_Window, GLFW_KEY_P) == GLFW_PRESS)
//        togglePolygonMode = !togglePolygonMode;
//}