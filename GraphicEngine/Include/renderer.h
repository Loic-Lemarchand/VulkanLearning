#pragma once

#include "window.h"
#include "pipeline.h"
#include "swap_chain.h"



//std
#include <memory>
#include <vector>
#include <cassert>



namespace Lve {
	class Renderer
	{
	public:

		Renderer(Window &window, Device &device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }
		float getAspectRatio() const { return lveSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const { 
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex]; }

		int getFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not int progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		
		
		
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();
		
		//void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);

		Window& lveWindow;
		Device& lveDevice;
		std::unique_ptr<SwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;

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