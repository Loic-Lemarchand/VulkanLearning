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

		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;//Every 3 vertices are connected into a seperate triangle
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.viewport.x = 0.0f;
		configInfo.viewport.y = 0.0f;//For the explanation of VIEWPORT and SCISSORS : https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Fixed_functions#page_Viewports-and-scissors
		configInfo.viewport.width = static_cast<float>(width);
		configInfo.viewport.height = static_cast<float>(height);
		configInfo.viewport.minDepth = 0.0f;
		configInfo.viewport.maxDepth = 1.0f;

		configInfo.scissor.offset = {0, 0};
		configInfo.scissor.extent = { width, height };

		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.pViewports = &configInfo.viewport;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pScissors = &configInfo.scissor;

		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; //Optional
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f; //Optional
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; //Optional
		
		return configInfo;
	}

}

