#include "nepch.h"
#include "Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_internal.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

#include "Editor/Editor.h"
#include "Execution/Executor.h"
#include "NodeGraph/NodeGraphCompiler.h"

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

	glfwSetFramebufferSizeCallback(m_Window, GLFWFramebufferSizeCallback);

	// Init ImGui
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Application::Run()
{
	bool editMode = true;
	bool editorInitialized = false;
	Scope<Editor> editor = CreateScope<Editor>();
	Scope<Executor> executor = CreateScope<Executor>();
	NodeGraphCompiler compiler;
	bool compilationSuccessful = false;

	static const std::string RUN_STR = "Run";
	static const std::string STOP_STR = "Stop";

	while (!glfwWindowShouldClose(m_Window))
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();

		// ImGui frame
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
			ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

			// Render
			{
				if (ImGui::Button("New") || !editorInitialized)
				{
					if (!editorInitialized)
						editor->Unload();

					editor->Load(NodeGraph::CreateDefault());
					editorInitialized = true;
				}

				ImGui::Separator();
				ImGui::SetNextItemWidth(250.0f);
				if (ImGui::Button(editMode ? RUN_STR.c_str() : STOP_STR.c_str()))
				{
					editMode = !editMode;

					if (!editMode)
					{
						CompiledPipeline pipeline = compiler.Compile(editor->GetNodeGraph());
						compilationSuccessful = compiler.GetErrorMessages().empty();
						if (compilationSuccessful)
						{
							executor->SetCompiledPipeline(pipeline);
							executor->OnStart();
						}
						else
						{
							for (const std::string& err : compiler.GetErrorMessages())
								std::cout << "[Compilation error] " << err << std::endl;
							editMode = true;
						}							
					}
				}
				ImGui::Separator();

				if (editMode)
					editor->Render();
			}

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
}
