#pragma once

#include "../GraphicEngine/Include/window.h"
#include "../GraphicEngine/Include/device.h"
#include "../GraphicEngine/Include/game_object.h"
#include "../GraphicEngine/Include/renderer.h"
#include "../GraphicEngine/Include/descriptors.h"


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
		void CreateGameObjectFromModel(std::shared_ptr<Model>& lveModel, const std::string &filepath);

	private:
		void loadGameObjects();

		Window lveWindow{ WIDTH, HEIGHT, "Maxwell the cat" };
		Device lveDevice{lveWindow};
		
		Renderer lveRenderer{ lveWindow, lveDevice };

	// Note : Order of declaration matters
		std::unique_ptr<DescriptorPool> globalPool{};
		GameObject::Map gameObjects;


		
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