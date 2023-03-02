#pragma once

#include "../camera.h"
#include "../pipeline.h"
#include "../device.h"
#include "../game_object.h"
#include "../frame_info.h"



//std
#include <memory>
#include <vector>



namespace Lve {
	class PointLightSystem
	{
	public:

		PointLightSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem &) = delete;
		PointLightSystem &operator=(const PointLightSystem &) = delete;

		void update(FrameInfo &frameInfo, GlobalUbo &ubo);
		void render(FrameInfo &frameInfo);

		
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
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

