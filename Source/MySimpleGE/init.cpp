#include <MySimpleGE/init.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <MySimpleGE/openglErrorReporting.h>
#include <MySimpleGE/Core/Resources/ResourceManager.h>
#include <MySimpleGE/Core/Resources/Material.h>
#include <MySimpleGE/Core/Resources/MeshResource.h>
#include <MySimpleGE/Core/Resources/Texture2dResource.h>
#include <MySimpleGE/Renderer/OpenGLRenderer.h>
#include <MySimpleGE/Core/Components/GLStaticMeshComponent.h>
#include <MySimpleGE/Core/Utils/Timer.h>

#include <MySimpleGE/Core/Singleton.h>

#include <nlohmann/json.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>


#include <iostream>
#include <fstream>
#include <algorithm> 

#pragma region imgui
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "imguiThemes.h"
#pragma endregion


namespace MSGE
{


// Function to handle mouse movement and WASD key events
void handleInput(SDL_Event& event, 
				int& mouseDeltaX, int& mouseDeltaY, 
				bool& moveForward, bool& moveBackward, bool& moveLeft, bool& moveRight) {
    switch (event.type) {
        case SDL_MOUSEMOTION:
            // Get mouse movement deltas
            mouseDeltaX = event.motion.xrel; // Relative x-axis movement
            mouseDeltaY = event.motion.yrel; // Relative y-axis movement
            //std::cout << "Mouse moved: (" << mouseDeltaX << ", " << mouseDeltaY << ")\n";
            break;

        case SDL_KEYDOWN:
            // Handle WASD key presses
            switch (event.key.keysym.sym) {
                case SDLK_w:
                    moveForward = true;
                    //std::cout << "W pressed\n";
                    break;
                case SDLK_a:
                    moveLeft = true;
                    //std::cout << "A pressed\n";
                    break;
                case SDLK_s:
                    moveBackward = true;
                    //std::cout << "S pressed\n";
                    break;
                case SDLK_d:
                    moveRight = true;
                    //std::cout << "D pressed\n";
                    break;
            }
            break;

        case SDL_KEYUP:
            // Handle WASD key releases
            switch (event.key.keysym.sym) {
                case SDLK_w:
                    moveForward = false;
                    //std::cout << "W released\n";
                    break;
                case SDLK_a:
                    moveLeft = false;
                    //std::cout << "A released\n";
                    break;
                case SDLK_s:
                    moveBackward = false;
                    //std::cout << "S released\n";
                    break;
                case SDLK_d:
                    moveRight = false;
                    //std::cout << "D released\n";
                    break;
            }
            break;

        default:
            break;
    }
}

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

	// After context creation, set vsync off
	if (SDL_GL_SetSwapInterval(0) != 0) {
		std::cerr << "Failed to disable vsync: " << SDL_GetError() << std::endl;
	}

	// Enable OpenGL error reporting
	enableReportGlErrors();

	OpenGLRenderer renderer;
	auto resManager = Singleton<ResourceManager>::getInstance();

	std::cout << "MAP COUNT REFERENCES:" << resManager->getCacheMap().size() << std::endl;
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

	
	std::vector<std::shared_ptr<Material>> newMaterials;

	auto carMesh = resManager->load<MeshResource>("car.json");
	auto carBodyTex = resManager->load<Texture2dResource>("car_body.png");
	auto carChasisTex = resManager->load<Texture2dResource>("car_chasis.png");

	auto matBody = resManager->load<Material>("car_body.mat.json");


	auto matChasis = resManager->load<Material>("car_chasis.mat.json");

	newMaterials.push_back(matBody);
	newMaterials.push_back(matChasis);
	
	GLStaticMeshComponent comp(&renderer, "car.json", newMaterials);
	comp.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.0f, 0.0f, -5.0f));
	comp.updateRequests();
	comp.addRequestsToRenderList();

	GLStaticMeshComponent comp2(&renderer, "car.json", newMaterials);
	comp2.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, -4.0f));
	comp2.updateRequests();
	comp2.addRequestsToRenderList();
	
	ScopedTimer st("SCENE LOAD: ");
	#pragma region sceneload

	std::ifstream f(ResourcePath("Scene1.scene.json"));
    if (f)
    {
		using json = nlohmann::json;
		std::vector<GLStaticMeshComponent> scene;

		json data = json::parse(f);
		auto objectsArrayValue = data["objects"];
		if (objectsArrayValue.is_array())
		{
			auto objectsArray = objectsArrayValue.get<json::array_t>();
			for (auto object : objectsArray)
			{
				if (object.is_object())
				{
					glm::mat4 transform(1.0);

					glm::mat4 translationMatrix;
					auto posVecValue = object["position"];
					if (posVecValue.is_array())
					{
						auto posVec = posVecValue.get<json::array_t>();
						glm::vec3 pos(1.0);
						for (int i = 0; i < std::min(3, (int)posVec.size()) ; i++)
						{
							pos[i] = posVec[i];
						}
						translationMatrix = glm::translate(glm::mat4(1.0f), pos);

					}

					glm::mat4 rotationMatrix;
					auto rotVecValue = object["rotation"];
					if (rotVecValue.is_array())
					{
						auto rotVec = rotVecValue.get<json::array_t>();
						glm::vec3 rot(1.0);
						for (int i = 0; i < std::min(3, (int)rotVec.size()) ; i++)
						{
							rot[i] = rotVec[i];
						}
						rotationMatrix = glm::yawPitchRoll(rot.y, rot.x, rot.z);
					}

					glm::mat4 scaleMatrix(1.0);
					
					auto scaleVecValue = object["scale"];
					if (scaleVecValue.is_array())
					{
						auto scaleVec = scaleVecValue.get<json::array_t>();
						glm::vec3 scale(1.0);
						for (int i = 0; i < std::min(3, (int)scaleVec.size()) ; i++)
						{
							scale[i] = scaleVec[i];
						}
						scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
					}

					transform = translationMatrix * rotationMatrix * scaleMatrix;

					ResourcePath meshPath;
					auto meshPathValue = object["mesh"];
					if (meshPathValue.is_string())
					{
						meshPath = meshPathValue.get<std::string>();
					}

					std::vector<std::shared_ptr<Material>> objectMaterials;
					auto matArrayValue = object["matOverrides"];
					if ( matArrayValue.is_array())
					{
						auto matArray = matArrayValue.get<json::array_t>();
						for (auto matPath : matArray)
						{
							if (matPath.is_string())
							{
								auto matRes = resManager->load<Material>( matPath.get<std::string>() );
								if (matRes)
								{
									objectMaterials.push_back(matRes);
								}
							}

						} 
					}
					GLStaticMeshComponent staticMeshComp(&renderer, meshPath, objectMaterials);
					staticMeshComp.modelMatrix = transform;
					staticMeshComp.updateRequests();
					staticMeshComp.addRequestsToRenderList();
				}
			}     
		}
	}

	#pragma endregion
	st.stop();

	float cameraYaw = 0.0;
	float cameraPitch = 0.0;
	glm::vec3 cameraPos = glm::vec3(0.0);
	
	// Main event loop
	bool running = true;
	while (running)
	{
		int w = 0, h = 0;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		// Variables to store input data
		int mouseDeltaX = 0, mouseDeltaY = 0;
		bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;

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

			handleInput(event, mouseDeltaX, mouseDeltaY, moveForward, moveBackward, moveLeft, moveRight);
		}

	#pragma region imgui
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		// Create a docking space
		//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	#pragma endregion
		
		float ws = 0.0;
		float ad = 0.0;
		if (moveForward)
			ws += 0.4;
		if (moveBackward)
			ws -= 0.4;
		if (moveLeft)
			ad += 0.4;
		if (moveRight)
			ad -= 0.4;

		auto inverted = glm::mat4(1.0);
		inverted[0][0] *= -1;
		inverted[1][1] *= -1;

		cameraYaw += mouseDeltaX*0.01;
		cameraPitch += mouseDeltaY*0.01;

		cameraPitch = std::clamp(cameraPitch, -1.5708f, 1.5708f);


		 // Create a quaternion for the rotation
    	glm::quat q1 = glm::angleAxis(cameraPitch, glm::vec3(1.0,0.0,0.0));
		// Create a quaternion for the rotation
    	glm::quat q2 = glm::angleAxis(cameraYaw, glm::vec3(0.0,1.0,0.0));

		// Convert quaternion to a 4x4 rotation matrix
    	glm::mat4 rotMat = glm::toMat4(q1*q2);

		renderer.viewMatrix = glm::translate (rotMat, cameraPos); 

		float amount = moveForward ? 0.15 : 0.0;
		glm::vec3 cameraDir = glm::inverse(rotMat) * glm::vec4(0.0,0.0,1.0, 1.0);   
		cameraPos += cameraDir * amount;
		
		float aspectRatio = (float)w/(float)h;
		glm::mat4 projection = glm::perspective(glm::radians(80.0f), aspectRatio, 0.01f, 1000.0f);
		renderer.projectionMatrix = projection;

		if (canRender == 0)
		{
			renderer.render();
		}


	#pragma region imgui
		// Example ImGui window
		ImGui::Begin("Test Window");
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::Text("Hello, world!");
		//ImGui::Button("I am Pressy the button!");
		//float values[5] = {0.5, 0.4, 0.3, 0.56, 0.46};
		//ImGui::PlotHistogram("I am a plot!", values, 5);
		
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
