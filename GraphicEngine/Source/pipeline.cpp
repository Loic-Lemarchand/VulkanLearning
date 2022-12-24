#include "../Include/pipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>


namespace Lve {

	

	Pipeline::Pipeline(
			Device& device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath,
		const PipelineConfigInfo& configInfo) : lveDevice{device} {
		createGraphicPipeline(vertFilepath, fragFilepath, configInfo);
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

	void Pipeline::createGraphicPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
	{
		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragFilepath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
	}

	void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());//a uint32 and a character are not the same size but the data is stored in a vector the default allocator insures that the data satisfy the worst cas alignement requirement 

		if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module");
		}
	}

	PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo configInfo{};

		return configInfo;
	}

}

