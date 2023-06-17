#include "nepch.h"
#include "Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

static void GLFWErrorCallback(int error, const char* description)
{
	NE_ERROR("GLFW Error ({0}): {1}", error, description);
}

static void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Application::Application()
{
	m_Width = 1024;
	m_Height = 720;

	// Init GLFW and create the widnow
	NE_INFO("Creating window ({0}x{1})", m_Width, m_Height);

	int success = glfwInit();
	NE_ASSERT(success, "Could not initialize GLFW!");
	glfwSetErrorCallback(GLFWErrorCallback);

#ifdef NE_DEBUG
	glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

	m_Window = glfwCreateWindow(m_Width, m_Height, "Node Editor", nullptr, nullptr);
	NE_ASSERT(m_Window, "Failed to create window!");
	if (!m_Window)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);

	// Init GLAD
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	NE_ASSERT(status, "Failed to initialize Glad!");

	NE_INFO("OpenGL Info: {0} {1}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));
	NE_INFO("  Vendor:   {0}", (const char*)glGetString(GL_VENDOR));
	NE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
	NE_INFO("  Version:  {0}", (const char*)glGetString(GL_VERSION));

	glfwSetFramebufferSizeCallback(m_Window, GLFWFramebufferSizeCallback);
}

Application::~Application()
{
	glfwTerminate();
}

void Application::Run()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}
