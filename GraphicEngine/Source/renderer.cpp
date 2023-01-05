#include "../Include/Renderer.h"

#include <stdexcept>
#include <array>


namespace Lve {


	Renderer::Renderer(Window &window, Device &device) : lveWindow(window), lveDevice(device)
	{
		recreateSwapChain();
		createCommandBuffers();
	}
	Renderer::~Renderer()
	{
		freeCommandBuffers();
	}



	void Renderer::recreateSwapChain()
	{
		auto extent = lveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = lveWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(lveDevice.device());

		if (lveSwapChain == nullptr)
		{
			lveSwapChain = std::make_unique<SwapChain>(lveDevice, extent);
		} else
		{
			std::shared_ptr<SwapChain> oldSwapChain = std::move(lveSwapChain);
			lveSwapChain = std::make_unique<SwapChain>(lveDevice, extent, std::move(lveSwapChain));

			if (!oldSwapChain->compareSwapFormats(*lveSwapChain.get()))
			{
				throw std::runtime_error("Swap Chain image (or depth) format has changed !");
			}

		
		}
		lveSwapChain = std::make_unique<SwapChain>(lveDevice, extent);
		
		//POSSIBLE IMPROVEMENT : if render pass compatible do nothing else
		
	}

	void Renderer::createCommandBuffers()
	{
		commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandPool = lveDevice.getCommandPool();
		allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate comman buffers");
		}

		
	}

	void Renderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			lveDevice.device(),
			lveDevice.getCommandPool(),
			static_cast<float>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	

	VkCommandBuffer Renderer::beginFrame()
	{
		assert(!isFrameStarted && "Can't call beginFrame while already in progress");

		
		auto result = lveSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to begin recording cmmand buffer");
			}

			return commandBuffer;
	}

	void Renderer::endFrame()
	{
		assert(isFrameStarted && "Can't call enFrame while frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to record command buffer");
			}


		auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			lveWindow.wasWindowwResized();
			lveWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call begin SwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = lveSwapChain->getRenderPass();
			renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(currentImageIndex);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();
			

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
			viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			VkRect2D scissor{ {0, 0}, lveSwapChain->getSwapChainExtent() };
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call end SwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);

	}

	//void Renderer::sierpinski(
	//	std::vector<Model::Vertex>& vertices,
	//	int depth,
	//	glm::vec2 left,
	//	glm::vec2 right,
	//	glm::vec2 top) {
	//	/*if (depth <= 0) {
	//		vertices.push_back({ top });
	//		vertices.push_back({ right });
	//		vertices.push_back({ left });
	//	}
	//	else {
	//		auto leftTop = 0.5f * (left + top);
	//		auto rightTop = 0.5f * (right + top);
	//		auto leftRight = 0.5f * (left + right);
	//		sierpinski(vertices, depth - 1, left, leftRight, leftTop);
	//		sierpinski(vertices, depth - 1, leftRight, right, rightTop);
	//		sierpinski(vertices, depth - 1, leftTop, rightTop, top);
	//	}*/
	//}

	
}