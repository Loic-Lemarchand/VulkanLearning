#pragma once

#include "../GraphicEngine/Include/window.h"
#include "../GraphicEngine/Include/pipeline.h"

namespace Lve {
class FirstApp
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	void run();
private:
	Window Window{WIDTH, HEIGHT, "Fenetre "};

	Pipeline Pipeline{ "shaders/vert.spv", "shaders/frag.spv" };
};

}