#pragma once

#include "camera.h"

#include <vulkan/vulkan.h>

namespace Lve
{
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera &camera;
    
    };
}

