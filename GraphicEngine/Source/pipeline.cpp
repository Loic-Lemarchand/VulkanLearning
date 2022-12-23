#include "../Include/pipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>


namespace Lve {

	Pipeline::Pipeline(const std::string& vertFilepath, const std::string& fragFilepath)
	{
		createGraphicPipeline(vertFilepath, fragFilepath);
	}

	std::vector<char> Pipeline::readFile(const std::string& filepath)
	{
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };// ate-> when the file is opened we seek to the end immediatly -- binary->we read it as a binary so we avoid any text transformation

		if (!file.is_open()) {//we fail to open the file
			throw std::runtime_error("failed to open file" + filepath);
		}
		//tellg give the postion, since we are at the end of the file thanks to "ate" it gives the file size
		size_t filesize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(filesize);

		file.seekg(0);
		file.read(buffer.data(), filesize);

		file.close();

		return buffer;
	}

	void Pipeline::createGraphicPipeline(const std::string& vertFilepath, const std::string& fragFilepath)
	{
		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
	}
}

