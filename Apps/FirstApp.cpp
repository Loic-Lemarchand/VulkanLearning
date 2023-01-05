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

	std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) {
	  std::vector<Model::Vertex> vertices{

		  // left face (white)
		  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		  {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
		  {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
		  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		  {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
		  {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

		  // right face (yellow)
		  {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		  {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
		  {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
		  {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		  {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
		  {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

		  // top face (orange, remember y axis points down)
		  {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		  {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		  {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		  {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		  {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		  {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

		  // bottom face (red)
		  {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		  {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
		  {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
		  {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		  {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		  {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

		  // nose face (blue)
		  {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		  {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		  {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		  {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		  {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		  {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

		  // tail face (green)
		  {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		  {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		  {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		  {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		  {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		  {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

	  };
	  for (auto& v : vertices) {
		v.position += offset;
	  }
	  return std::make_unique<Model>(device, vertices);
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<Model> lveModel = createCubeModel(lveDevice, { .0f, .0f, .0f });

		auto cube = GameObject::createGameObject();
		cube.model = lveModel;
		cube.transform.translation = { .0f, .0f, .5f };
		cube.transform.scale = { .5f, .5f, .5f };

		gameObjects.push_back(std::move(cube));

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