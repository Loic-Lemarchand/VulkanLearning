#pragma once


#include "../GraphicEngine/Include/pipeline.h"
#include "../GraphicEngine/Include/device.h"
#include "../GraphicEngine/Include/game_object.h"



//std
#include <memory>
#include <vector>



namespace Lve {
	class SimpleRenderSystem
	{
	public:

		SimpleRenderSystem(Device &device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		void renderGameObjects(VkCommandBuffer commendBuffer, std::vector<GameObject>& gameObjects);

		
	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		

		Device &lveDevice;
		std::unique_ptr<Pipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;

	/*	Pipeline lvePipeline{
			lveDevice,
			"shaders/vert.spv",
			"shaders/frag.spv",
			Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
	};*/

		/*void sierpinski(std::vector<Model::Vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top);*/
		
};

}

