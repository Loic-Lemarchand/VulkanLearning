#pragma once

#include <string>
#include <vector>

namespace Lve {
class Pipeline
{
public:
	Pipeline(const std::string& vertFilepath, const std::string& fragFilepath);

private:
	static std::vector<char> readFile(const std::string& filePath);

	void createGraphicPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
};

}