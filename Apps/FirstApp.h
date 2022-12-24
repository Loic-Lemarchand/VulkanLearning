#pragma once

#include "../GraphicEngine/Include/window.h"
#include "../GraphicEngine/Include/pipeline.h"
#include "../GraphicEngine/Include/device.h"

namespace Lve {
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		Window lveWindow{ WIDTH, HEIGHT, "Vulkan !!!" };
		Device lveDevice{lveWindow};

		Pipeline lvePipeline{
			lveDevice,
			"shaders/vert.spv",
			"shaders/frag.spv",
			Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
	};
};

}