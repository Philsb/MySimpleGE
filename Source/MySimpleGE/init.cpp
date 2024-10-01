#include <MySimpleGE/init.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <MySimpleGE/openglErrorReporting.h>
#include <MySimpleGE/Renderer/OpenGLRenderer.h>
#include <MySimpleGE/Core/Resources/ResourceManager.h>
#include <MySimpleGE/Core/Resources/MeshResource.h>
#include <MySimpleGE/Core/Resources/Texture2dResource.h>

#include <MySimpleGE/Renderer/GLMeshBuffer.h>
#include <MySimpleGE/Renderer/GLTexture2d.h>
#include <MySimpleGE/Renderer/GLSLShader.h>
#include <MySimpleGE/Renderer/GLStaticModelRenderRequest.h>

#include <MySimpleGE/Core/Singleton.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma region imgui
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "imguiThemes.h"
#pragma endregion


namespace MSGE
{
int init_engine()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

#ifdef __APPLE__
	// apple moment...
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

	// Create a window
	SDL_Window *window = SDL_CreateWindow("Hello SDL", 100, 100, 680, 480,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Create OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Load OpenGL functions using glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Enable OpenGL error reporting
	enableReportGlErrors();

	OpenGLRenderer renderer;
	auto resManager = Singleton<ResourceManager>::getInstance();

	/*{
		std::shared_ptr<Mesh> mesh_ptr1 = resManager->load<Mesh>("BRUHHHHH");
		std::shared_ptr<Mesh> mesh_ptr2 = resManager->load<Mesh>("BRUHHHHH");

		std::cout << "IMAGE Vertex:" << mesh_ptr2->getVertices()[0].position.x << std::endl;
	}

	std::cout << "MAP COUNT REFERENCES:" << resManager->getCacheMap().size() << std::endl;*/
	int canRender = renderer.init((GLADloadproc)SDL_GL_GetProcAddress);

#pragma region imgui
	ImGui::CreateContext();

	//you can use whatever imgui theme you like!
	imguiThemes::yellow();
	//ImGui::StyleColorsDark();
	//imguiThemes::gray();
	//imguiThemes::green();
	//imguiThemes::red();
	//imguiThemes::embraceTheDarkness();


	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.f;
		style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330");
#pragma endregion

	const std::string vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNorm;\n"
	"layout (location = 2) in vec2 aUV1;\n"
    "uniform mat4 modelMat; \n"
    "uniform mat4 projMat; \n"
    "smooth out vec3 vertexNormal; \n"
	"smooth out vec2 texCoord1; \n"
    "void main()\n"
    "{\n"
	"   texCoord1 = aUV1;\n"
    "   vertexNormal = (modelMat * vec4(aNorm,1.0)).xyz;\n"
    "   gl_Position = projMat * modelMat * vec4(aPos, 1.0);\n"
    "}\0";
    const std::string fragmentShaderSource = "#version 330 core\n"
    "smooth in vec3 vertexNormal;\n"
	"smooth in vec2 texCoord1; \n"
    "out vec4 FragColor;\n"
	"uniform sampler2D albedo;\n"
    "void main()\n"
    "{\n"
	"	vec4 texColor = texture(albedo, texCoord1);\n"
    "   vec3 finalColor = texColor.rgb * mix (0.2, 1.0, (1.0 + dot(vertexNormal, vec3(0.0,1.0,0.0))) * 0.5);\n"
    "   FragColor = vec4(finalColor, 1.0f);\n"
    "}\n\0";
	

    auto meshBuffer1 = renderer.allocateGLResource<GLMeshBuffer>("Mesh1");
    //auto mesh_ptr1 = resManager->load<Mesh>("Mesh1");

	

	MeshResource mr;
	mr.load(RESOURCES_PATH + std::string("car.json"));

	Texture2dResource t2dr;
	t2dr.load(RESOURCES_PATH + std::string("img.png")); 

	auto tex1 = renderer.allocateGLResource<GLTexture2d>("Tex1");
	tex1->setData(t2dr.getWidth() , t2dr.getHeight(), t2dr.getImageData() );

    meshBuffer1->setData(mr.getMeshData().getVertices(), mr.getMeshData().getIndices());

    auto shader = renderer.allocateGLResource<GLSLShader>("Shader1");
    shader->setAndCompileShader(vertexShaderSource, fragmentShaderSource);

	/*
    auto meshRenderReq1 = std::make_shared<GLStaticModelRenderRequest>();
    meshRenderReq1->setMesh(meshBuffer1);
    meshRenderReq1->setShader(shader);
    meshRenderReq1->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)));
	*/

	auto meshRenderReq2 = std::make_shared<GLStaticModelRenderRequest>();
    meshRenderReq2->setMesh(meshBuffer1);
    meshRenderReq2->setShader(shader);
    meshRenderReq2->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, -5.0f)));
	meshRenderReq2->setTextureUniform("albedo",0,tex1);
    //renderer.addToRenderList(meshRenderReq1); 
	renderer.addToRenderList(meshRenderReq2); 

	// Main event loop
	bool running = true;
	while (running)
	{
		int w = 0, h = 0;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
			{
				running = false;
			}

			if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_CLOSE && 
					event.window.windowID == SDL_GetWindowID(window))
				{
					running = false;
				}
			}
		}

	#pragma region imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		// Create a docking space
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	#pragma endregion


		/*glm::mat4 modelMat1 = meshRenderReq1->getModelMatrix();
		meshRenderReq1->setModelMatrix(glm::rotate(modelMat1, glm::radians(4.0f), glm::vec3(1.0f, 0.3f, 0.5f)));*/

		glm::mat4 modelMat2 = meshRenderReq2->getModelMatrix();
		meshRenderReq2->setModelMatrix(glm::rotate(modelMat2, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f)));
		
		
		if (canRender == 0)
		{
			renderer.render();
		}


	#pragma region imgui
		// Example ImGui window
		ImGui::Begin("Test Window");
		
		ImGui::Text("Hello, world!");
		ImGui::Button("I am Pressy the button!");
		float values[5] = {0.5, 0.4, 0.3, 0.56, 0.46};
		ImGui::PlotHistogram("I am a plot!", values, 5);
		
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//view port stuff
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
	#pragma endregion

		SDL_GL_SwapWindow(window);
	}

	renderer.printResources();

	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// Cleanup SDL
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}


}
