#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "../Input/Input.h"

class Window
{
public:
	Window(Input* inputHandler);

    void key_callback(int key, int scancode, int actions, int mods)
    {
        m_InputHandler->KeyCallback(m_Window, key, scancode, actions, mods);
    }

    GLFWwindow* GetGLFWWindow();
    Input* GetInputHandler();
    
    void CheckKeyInput();

private:
	GLFWwindow* m_Window;
    Input* m_InputHandler;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
            win->key_callback(key, scancode, action, mods);
    }
};

#endif // !WINDOW_H
