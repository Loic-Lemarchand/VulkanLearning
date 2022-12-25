#include "FirstApp.h"

#include <stdexcept>


namespace Lve {

	FirstApp::FirstApp()
	{
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}
	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run()
	{
		while (!lveWindow.shoudlClose())
		{
			glfwPollEvents();
		}
	}

	void FirstApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void FirstApp::createPipeline()
	{
		auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
		pipelineConfig.renderPass = lveSwapChain.getRenderPass();

		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<Pipeline>(
			lveDevice,
			"shaders/vert.spv",
			"shaders/frag.spv",
			pipelineConfig);

	}

	void FirstApp::createCommandBuffers()
	{

	}
	void FirstApp::drawFrame()
	{

	}
}