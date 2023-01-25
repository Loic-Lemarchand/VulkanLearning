#include "FirstApp.h"
#include "../GraphicEngine/Include/simple_render_system.h"
#include "../GraphicEngine/Include/camera.h"
#include "../GraphicEngine/Include/keyboard_movement_controller.h"

// std
#include <stdexcept>
#include <array>
#include <chrono>
#include <cassert>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>




namespace Lve {


	FirstApp::FirstApp()
	{
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };
		Camera camera{};

		////camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
		//camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = GameObject::createGameObject();
		KeyboardMovementController comeraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		//object movement
		float initPosY = gameObjects[0].transform.translation.y;
		
		//AudioSystem::SendEvent("rupee_drop");
		
		while (!lveWindow.shoudlClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			

			comeraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);//
			
			float aspect = lveRenderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			gameObjects[0].transform.rotation.y += 0.002f;

			gameObjects[0].transform.translation.y = sin(frameTime)/2;
			gameObjects[0].transform.translation.x = cos(frameTime)/2;

			
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}
	
	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<Model> lveModel = Model::createModelFromFile(lveDevice, "Models/maxwell.obj");


		auto gameObj = GameObject::createGameObject();
		gameObj.model = lveModel;
		gameObj.transform.translation = { .0f, .0f, 2.f };
		gameObj.transform.scale = { .1f, .1f, .1f };



		gameObjects.push_back(std::move(gameObj));

	}



	/*void FirstApp::sierpinski(
		std::vector<Model::Vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top) {
		if (depth <= 0) {
			vertices.push_back({ top });
			vertices.push_back({ right });
			vertices.push_back({ left });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			sierpinski(vertices, depth - 1, left, leftRight, leftTop);
			sierpinski(vertices, depth - 1, leftRight, right, rightTop);
			sierpinski(vertices, depth - 1, leftTop, rightTop, top);
		}
	}*/

	
}