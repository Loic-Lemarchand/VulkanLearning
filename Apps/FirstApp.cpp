#include "FirstApp.h"
#include "../GraphicEngine/Include/simple_render_system.h"

#include <stdexcept>
#include <array>

//libs
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
		while (!lveWindow.shoudlClose())
		{
			glfwPollEvents();
			
			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		/*std::vector<Model::Vertex> vertices{};
		sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
		lveModel = std::make_unique<Model>(lveDevice, vertices);*/

		std::vector<Model::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		auto lveModel = std::make_shared<Model>(lveDevice, vertices);
	
		auto triangle = GameObject::createGameObject();
		triangle.model = lveModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform2D.translation.x = .2f;
		triangle.transform2D.scale = { 2.f, .5f };
		triangle.transform2D.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));

	}



	void FirstApp::sierpinski(
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
	}

	
}