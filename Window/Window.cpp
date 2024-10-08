#include "Window.h"

bool left = false;

Window::Window(Input* inputHandler, unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height)
{
	m_Window = glfwCreateWindow(m_Width, m_Height, "mc-gl", NULL, NULL);
	m_InputHandler = inputHandler;
	m_AspectRatio = (float)(m_Width) / m_Height;
	glfwGetCursorPos(m_Window, &m_CursorPos.x, &m_CursorPos.y);
	m_LastCursorPos = m_CursorPos;

	glfwSetWindowUserPointer(m_Window, this);

	glfwMakeContextCurrent(m_Window);

	//glfwSetKeyCallback(m_Window, key_callback);

	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_FALSE);
	glfwSetInputMode(m_Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

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

void Window::CheckInput()
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

	//if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
	//	cam->Rotate(GLFW_KEY_UP);
	//if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
	//	cam->Rotate(GLFW_KEY_DOWN);
	//if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
	//	cam->Rotate(GLFW_KEY_LEFT);
	//if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	//	cam->Rotate(GLFW_KEY_RIGHT);

	CursorMovement();

	if (glfwGetKey(m_Window, GLFW_KEY_P) == GLFW_PRESS)
		m_InputHandler->TogglePolygonMode();

	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_PRESS)
		m_InputHandler->GetPlayer()->ChangeState(STATE_BREAK);
	else
		m_InputHandler->GetPlayer()->ChangeState(STATE_WATCH);
}

void Window::CursorMovement()
{
	// detect cursor movement only if it's inside the window
	if (!glfwGetWindowAttrib(m_Window, GLFW_HOVERED))
		return;

	glfwGetCursorPos(m_Window, &m_CursorPos.x, &m_CursorPos.y);

	if (&m_CursorPos.x == NULL || &m_CursorPos.y == NULL)
	{
		return;
	}

	if (m_CursorPos.x <= 0.0 || m_CursorPos.x >= (float)(m_Width - 1) || m_CursorPos.y <= 0.0 || m_CursorPos.y >= (float)(m_Height - 1))
	{
		if (!left)
		{
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPos(m_Window, m_CursorPos.x + 1.0, m_CursorPos.y + 1.0);
		}

		left = true;
	}
	else
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		left = false;
	}

	if (m_CursorPos.x == m_LastCursorPos.x && m_CursorPos.y == m_LastCursorPos.y)
		return;

	Camera* cam = m_InputHandler->GetPlayer()->GetCam();

	cursor deltaPos;
	deltaPos.x = m_CursorPos.x - m_LastCursorPos.x;
	deltaPos.y = m_CursorPos.y - m_LastCursorPos.y;

	if (deltaPos.x > CURSOR_DELTA_MOVEMENT)
		cam->Rotate(GLFW_KEY_RIGHT);
	else if(deltaPos.x < -CURSOR_DELTA_MOVEMENT)
		cam->Rotate(GLFW_KEY_LEFT);

	if (deltaPos.y > CURSOR_DELTA_MOVEMENT)
		cam->Rotate(GLFW_KEY_DOWN); // downward y axis in screen coordinate system
	else if(deltaPos.y < -CURSOR_DELTA_MOVEMENT)
		cam->Rotate(GLFW_KEY_UP);

	m_LastCursorPos = m_CursorPos;
}