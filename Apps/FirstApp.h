#pragma once

#include "../GraphicEngine/Include/window.h"
#include "../GraphicEngine/Include/device.h"
#include "../GraphicEngine/Include/game_object.h"
#include "../GraphicEngine/Include/renderer.h"


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
		void loadGameObjects();

		Window lveWindow{ WIDTH, HEIGHT, "Vulkan !!!" };
		Device lveDevice{lveWindow};
		std::vector<GameObject> gameObjects;
		Renderer lveRenderer{ lveWindow, lveDevice };

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