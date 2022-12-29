#pragma once

#include "../GraphicEngine/Include/window.h"
#include "../GraphicEngine/Include/pipeline.h"
#include "../GraphicEngine/Include/device.h"
#include "../GraphicEngine/Include/swap_chain.h"
#include "../GraphicEngine/Include/model.h"


//std
#include <memory>
#include <vector>



namespace Lve {
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		Window lveWindow{ WIDTH, HEIGHT, "Vulkan !!!" };
		Device lveDevice{lveWindow};
		SwapChain lveSwapChain{ lveDevice, lveWindow.getExtent()};
		std::unique_ptr<Pipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::unique_ptr<Model> lveModel;

	/*	Pipeline lvePipeline{
			lveDevice,
			"shaders/vert.spv",
			"shaders/frag.spv",
			Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
	};*/

		void sierpinski(std::vector<Model::Vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top);
		
};

}