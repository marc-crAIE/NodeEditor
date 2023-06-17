#pragma once

#include <string>

struct GLFWwindow;

class Application
{
public:
	Application();
	~Application();

	void Run();
private:
	int m_Width;
	int m_Height;

	GLFWwindow* m_Window;
};