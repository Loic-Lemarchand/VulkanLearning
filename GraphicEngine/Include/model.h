#pragma once

#include "device.h"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace Lve {

	//The purpose of this class is to take vertex data and allocate the memory and copy the data over to our device's GPU so it can be rendred efficiently
	//We go for Interlieving position with color attribute
	class Model
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();

			bool operator==(const Vertex& other) const
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string &filepath);
		};

		Model(Device &device, const Model::Builder &builder);
		~Model();

		static std::unique_ptr<Model> createModelFromFile(Device &device, const std::string &filepath);

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);
		void createIndexBuffers(const std::vector<uint32_t> &indices);
		
		Device& lveDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;


		bool hasIndexBuffer = false;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;


	};
}

