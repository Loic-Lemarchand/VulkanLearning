﻿#pragma once

#include "camera.h"
#include "game_object.h"

#include <vulkan/vulkan.h>

namespace Lve
{
#define  MAX_LIGHTS 10

    struct PointLight
    {
        glm::vec4 position{}; //ignore w
        glm::vec4 color{}; //w is intensity
    };
    struct GlobalUbo
    {
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::mat4 inverseView{1.f};
        glm::vec4 ambientLightColor{1.f,1.f,1.f, .02f}; // w intensity
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };
    
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera &camera;
        VkDescriptorSet globalDescriptorSet;
        GameObject::Map &gameObjects;
    };
}

