#include "FirstApp.h"
#include "../GraphicEngine/Include/Systems/simple_render_system.h"
#include "../GraphicEngine/Include/camera.h"
#include "../GraphicEngine/Include/keyboard_movement_controller.h"
#include "../GraphicEngine/Include/Systems/point_light_system.h"

// std
#include <stdexcept>
#include <array>
#include <chrono>
#include <cassert>
#include <numeric>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <filesystem>
#include <glm/glm.hpp>
#include "../GraphicEngine/Include/buffer.h"
#include <glm/gtc/constants.hpp>
#include <iostream>








namespace Lve {

	


	FirstApp::FirstApp()
	{
		globalPool = DescriptorPool::Builder(lveDevice)
		.setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
		.build();
		
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<Buffer>(
				lveDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}


		auto globalSetLayout = DescriptorSetLayout::Builder(lveDevice)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
		.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			DescriptorWriter(*globalSetLayout, *globalPool)
			.writeBuffer(0, &bufferInfo)
			.build(globalDescriptorSets[i]);
		}
		
		SimpleRenderSystem simpleRenderSystem{
			lveDevice,
			lveRenderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout() };
		PointLightSystem pointLightSystem{
			lveDevice,
			lveRenderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout() };
		Camera camera{};

		////camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
		//camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewerObject = GameObject::createGameObject();
		viewerObject.transform.translation.z = -4.f;
		KeyboardMovementController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		/*//object movement
		float initPosY = gameObjects[0].transform.translation.y;*/
		
		//AudioSystem::SendEvent("rupee_drop");
		//float  MAX_FRAME_TIME = 0.003333;
		
		while (!lveWindow.shoudlClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			//frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			//Display our FrameTime (framerate = 1/frameTime)
			//std::cout << frameTime << std::endl;

			cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime*10000, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);//
			
			float aspect = lveRenderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			
			//gameObjects[0].transform.rotation.y += 0.002f;
			
			/*gameObjects[0].transform.translation.y = sin(frameTime)/2;
			gameObjects[0].transform.translation.x = cos(frameTime)/2;*/
			
			
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				int frameIndex = lveRenderer.getFrameIndex();
				FrameInfo frameInfo{
				frameIndex,
				frameTime,
				commandBuffer,
				camera,
				globalDescriptorSets[frameIndex],
				gameObjects};
				
				//update
				GlobalUbo ubo{};
				ubo.projection= camera.getProjection() ;
				ubo.view= camera.getView() ;
				ubo.inverseView = camera.getInverseView();
				pointLightSystem.update(frameInfo, ubo);

				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();


				//Render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}
	
	void FirstApp::loadGameObjects()
	{
		//maxwell
		std::shared_ptr<Model> lveModel = Model::createModelFromFile(lveDevice, "Models/maxwell.obj");


		auto gameObj = GameObject::createGameObject();
		gameObj.model = lveModel;
		gameObj.transform.translation = { 0.f, 0.f, 0.f };
		gameObj.transform.scale = { .05f, .05f, .05f };
		
		gameObjects.emplace(gameObj.getId(),std::move(gameObj));

		//vase
		lveModel = Model::createModelFromFile(lveDevice, "Models/smooth_vase.obj");
		auto vase = GameObject::createGameObject();
		vase.model = lveModel;
		vase.transform.translation = { -2.f, 0.5f, -1.f };
		vase.transform.scale = { 5.f, 5.f, 5.f };
		
		gameObjects.emplace(vase.getId(), std::move(vase));

		//ground
		lveModel = Model::createModelFromFile(lveDevice, "Models/quad.obj");
		auto floor = GameObject::createGameObject();
		floor.model = lveModel;
		floor.transform.translation = { 0.f, .5f, 0.f };
		floor.transform.scale = { 3.f, 1.f, 3.f };
		
		gameObjects.emplace(floor.getId(), std::move(floor));
		

		
		/*auto pointLight = GameObject::makePointLight(0.2f);
		gameObjects.emplace(pointLight.getId(), std::move(pointLight));*/


		// using pointLight again invalid...
		std::vector<glm::vec3> lightColors{
	      {1.f, .1f, .1f},
		  {.1f, .1f, 1.f},
		  {.1f, 1.f, .1f},
		  {1.f, 1.f, .1f},
		  {.1f, 1.f, 1.f},
		  {1.f, 1.f, 1.f}  //
		};

		for (int i = 0; i < lightColors.size(); i++)
		{
			auto pointLight = GameObject::makePointLight(0.2f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(
				glm::mat4(1.f),
				(i * glm::two_pi<float>()) / lightColors.size(),
				{0.f, -1.f, 0.f});
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
			gameObjects.emplace(pointLight.getId(), std::move(pointLight));
		}
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