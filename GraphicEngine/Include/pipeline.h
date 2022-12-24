#pragma once

#include "device.h"


// std
#include <string>
#include <vector>



namespace Lve {

struct PipelineConfigInfo {};


class Pipeline
{
	public:
		Pipeline(
			Device& device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);

	~Pipeline() {}

	Pipeline(const Pipeline&) = delete;
	void operator=(const Pipeline&) = delete;

	static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		//Pipeline(Device device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicPipeline(
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);


		Device& lveDevice;//Outlive all the instances of the containing class

		VkPipeline graphicPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
};

}