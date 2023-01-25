#include "../Include/simple_render_system.h"

#include <stdexcept>
#include <array>

//libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace Lve {

	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.f };//identity matrix
		alignas(16) glm::vec3 color;
	};

	SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderpass) : lveDevice{device}
	{
		createPipelineLayout();
		createPipeline(renderpass);
	}
	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}


	void SimpleRenderSystem::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);



		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		/*
		auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(lveSwapChain->width(), lveSwapChain->height());
		
		*/
		 PipelineConfigInfo pipelineConfig{};
		Pipeline::defaultPipelineConfigInfo(
      pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<Pipeline>(
			lveDevice,
			"x64/Debug/shaders/simple_shader.vert.spv",
			"x64/Debug/shaders/simple_shader.frag.spv",
			pipelineConfig);

	}

	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera)
	{
		lvePipeline->bind(commandBuffer);

		auto projectionView = camera.getProjection() * camera.getView();

		for (auto& obj : gameObjects)
		{
			/*obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.001f, glm::two_pi<float>());
			obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.0005f, glm::two_pi<float>());*/

			SimplePushConstantData push{};
			push.color = obj.color;
			push.transform = projectionView*obj.transform.mat4();

				vkCmdPushConstants(
					commandBuffer,
					pipelineLayout,
					VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
					sizeof(SimplePushConstantData),
					&push);
				obj.model->bind(commandBuffer);
				obj.model->draw(commandBuffer);

		}
	}



	/*void SimpleRenderSystem::sierpinski(
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