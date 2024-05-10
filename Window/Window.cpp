#include "Window.h"

Window::Window(Input* inputHandler)
{
	m_Window = glfwCreateWindow(1280, 960, "mc-gl", NULL, NULL);
	m_InputHandler = inputHandler;
	glfwSetWindowUserPointer(m_Window, this);

	glfwMakeContextCurrent(m_Window);

	//glfwSetKeyCallback(m_Window, key_callback);

	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_FALSE);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
}

GLFWwindow* Window::GetGLFWWindow()
{
	return m_Window;
}

Input* Window::GetInputHandler()
{
	return m_InputHandler;
}

void Window::CheckKeyInput()
{
	Camera* cam = m_InputHandler->GetPlayer()->GetCam();

	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		cam->Move(GLFW_KEY_W);
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		cam->Move(GLFW_KEY_A);
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		cam->Move(GLFW_KEY_D);
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		cam->Move(GLFW_KEY_S);

	if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
		cam->Rotate(GLFW_KEY_UP);
	if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam->Rotate(GLFW_KEY_DOWN);
	if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam->Rotate(GLFW_KEY_LEFT);
	if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam->Rotate(GLFW_KEY_RIGHT);

	if (glfwGetKey(m_Window, GLFW_KEY_P) == GLFW_PRESS)
		m_InputHandler->TogglePolygonMode();

	m_InputHandler->GetPlayer()->UpdateChunkGridPosition();
}