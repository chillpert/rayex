#include "utils/VkUtils.hpp"

namespace RX
{
  VkInstance createInstance
  (
    std::shared_ptr<Window> window
  )
  {
    VkApplicationInfo appInfo = { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

#ifdef RX_DEBUG
    std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

    // Checks if given validation layers are available on this device.
    uint32_t propertyCount;
    vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);

    std::vector<VkLayerProperties> properties(propertyCount);
    vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());

    for (const auto& layerName : validationLayers)
    {
      bool found = false;

      for (const auto& property : properties)
      {
        std::cout << layerName << " : " << property.layerName << std::endl;

        if (strcmp(property.layerName, layerName))
        {
          found = true;
          break;
        }
      }

      if (!found)
      {
        Error::runtime("Validation layer is not available on this device", Error::API);
      }
    }

    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
#endif

    // Retrieves all extensions needed by SDL2 and add them to all extensions that will be used.
    uint32_t sdlExtensionsCount;
    window->getInstanceExtensions(sdlExtensionsCount, NULL);
    const char** sdlExtensionsNames = new const char*[sdlExtensionsCount];
    window->getInstanceExtensions(sdlExtensionsCount, sdlExtensionsNames);

    std::vector<const char*> extensions(sdlExtensionsNames, sdlExtensionsNames + sdlExtensionsCount);

#ifdef RX_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

    VkInstance instance;

    Assert::vulkan(
      vkCreateInstance(&createInfo, nullptr, &instance),
      "Failed to create instance"
    );

    return instance;
  }

  VkPhysicalDevice pickPhysicalDevice
  (
    VkInstance instance
  )
  {
    uint32_t physicalDeviceCount = 0;
    Assert::vulkan(
      vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr),
      "Failed to enumerate physical devices"
    );

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    Assert::vulkan(
      vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()),
      "Failed to enumerate physical devices"
    );

    for (uint32_t i = 0; i < physicalDeviceCount; ++i)
    {
      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(physicalDevices[i], &props);

      if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
      {
#ifdef RX_DEBUG
        std::cout << "Vk: GPU: " << props.deviceName << std::endl;
#endif
        return physicalDevices[i];
      }
    }

    // Any non-discrete GPU will not have enough performance to do path tracing at all.
    Error::runtime("No discrete GPU with Vulkan support available", Error::API);
  }

  VkDevice createDevice
  (
    VkInstance instance, 
    VkPhysicalDevice physicalDevice,
    uint32_t* familyIndex
  )
  {
    VkDevice device;

    *familyIndex = 0;

    uint32_t queueFamilyPropertyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

    // Iterates over all available queue families and find one that supports the graphics bit.
    for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i)
    {
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        *familyIndex = i;
        break;
      }
    }

    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueInfo.queueFamilyIndex = *familyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;

    std::vector<const char*> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    Assert::vulkan(
      vkCreateDevice(physicalDevice, &createInfo, nullptr, &device),
      "Failed to create device"
    );

    return device;
  }

  VkSwapchainKHR createSwapChain
  (
    VkPhysicalDevice physicalDevice, 
    VkDevice device, 
    VkSurfaceKHR surface,
    std::shared_ptr<Window> window, 
    uint32_t* familyIndex,
    VkFormat* format
  )
  {
    VkSwapchainKHR swapChain;

    VkFormatProperties formatProperties = { };
    vkGetPhysicalDeviceFormatProperties(physicalDevice, *format, &formatProperties);
    // TODO: check if this color format is supported

    VkSwapchainCreateInfoKHR createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    VkBool32 supported = false;
    Assert::vulkan(
      vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, *familyIndex, surface, &supported),
      "Failed to query pyhsical device surface support"
    );

    if (supported == VK_FALSE)
    {
      Error::runtime("Physical device surface does not support WSI", Error::API);
    }

    createInfo.surface = surface;

    createInfo.minImageCount = 2;

    uint32_t surfaceFormatCounts;
    Assert::vulkan(
      vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCounts, nullptr),
      "Failed to query physical device surface formats"
    );

    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCounts);
    Assert::vulkan(
      vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCounts, surfaceFormats.data()),
      "Failed to query physical device surface formats"
    );

    bool accepted = false;
    for (const auto& iter : surfaceFormats)
    {
      if (iter.format == *format && iter.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        createInfo.imageFormat = iter.format;
        createInfo.imageColorSpace = iter.colorSpace;

        accepted = true;
        break;
      }
    }

    // If the wanted format and space are not available, fall back.
    if (!accepted)
    {
      createInfo.imageFormat = surfaceFormats[0].format;
      createInfo.imageColorSpace = surfaceFormats[0].colorSpace;

      *format = surfaceFormats[0].format;
    }

    VkSurfaceCapabilitiesKHR surfaceCapabilitites;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilitites);
    
    createInfo.preTransform = surfaceCapabilitites.currentTransform; // causes error?!
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    int width, height;
    window->getWindowSize(&width, &height);
    createInfo.imageExtent.width = static_cast<uint32_t>(width);
    createInfo.imageExtent.height = static_cast<uint32_t>(height);

    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = familyIndex;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

    Assert::vulkan(
      vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain),
      "Failed to create swapchain"
    );

    return swapChain;
  }
 
  VkSemaphore createSemaphore
  (
    VkDevice device
  )
  {
    VkSemaphore semaphore;
    
    VkSemaphoreCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    Assert::vulkan(
      vkCreateSemaphore(device, &createInfo, nullptr, &semaphore),
      "Failed to create semaphore"
    );

    return semaphore;
  }

  VkCommandPool createCommandPool
  (
    VkDevice device, 
    uint32_t* familyIndex
  )
  {
    VkCommandPool commandPool;

    VkCommandPoolCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    createInfo.queueFamilyIndex = *familyIndex;

    Assert::vulkan(
      vkCreateCommandPool(device, &createInfo, nullptr, &commandPool),
      "Failed to create command pool"
    );

    return commandPool;
  }

  VkRenderPass createRenderPass
  (
    VkDevice device, 
    VkFormat format
  )
  {
    VkRenderPass renderPass;

    VkAttachmentDescription colorAttachmentDescription = { };
    colorAttachmentDescription.format = format;
    colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentReference = { };
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = { };
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;

    VkRenderPassCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &colorAttachmentDescription;
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpassDescription;
    
    Assert::vulkan(
      vkCreateRenderPass(device, &createInfo, nullptr, &renderPass),
      "Failed to create render pass"
    );

    return renderPass;
  }

  VkPipeline createPipeline
  (
    VkDevice device, 
    VkRenderPass renderPass,
    std::shared_ptr<Window> window,
    std::shared_ptr<Shader> vertex,
    std::shared_ptr<Shader> fragment
  )
  {
    VkPipeline pipeline;

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = { };
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = *vertex->getShaderModule();
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = { };
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = *fragment->getShaderModule();
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = { };
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = { };
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{ };
    viewport.x = 0.0f;
    viewport.y = 0.0f;

    int width, height;
    window->getWindowSize(&width, &height);
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = { };
    scissor.offset = { 0, 0 };
    VkExtent2D temp;
    temp.height = height;
    temp.width = width;
    scissor.extent = temp;

    VkPipelineViewportStateCreateInfo viewportState = { };
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = { };
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = { };
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = { };
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = { };
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = { };
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    VkPipelineLayout pipelineLayout;
    Assert::vulkan(
      vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout),
      "Failed to create pipeline layout"
    );

    VkGraphicsPipelineCreateInfo pipelineInfo = { };
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    // TODO: pipelinecreateinfo should be an array
    Assert::vulkan(
      vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline),
      "Failed to create graphics pipeline"
    );

    return pipeline;
  }

  VkFramebuffer createFramebuffer
  (
    VkDevice device, 
    VkRenderPass renderPass, 
    VkImageView imageView, 
    std::shared_ptr<Window> window
  )
  {
    VkFramebuffer framebuffer;

    VkFramebufferCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.renderPass = renderPass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &imageView;

    int width, height;
    window->getWindowSize(&width, &height);
    createInfo.width = static_cast<uint32_t>(width);
    createInfo.height = static_cast<uint32_t>(height);

    createInfo.layers = 1;

    Assert::vulkan(
      vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffer),
      "Failed to create frame buffer"
    );

    return framebuffer;
  }

  VkImageView createImageView
  (
    VkDevice device,
    VkImage image, 
    VkFormat format
  )
  {
    VkImageView imageView;

    VkImageViewCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;
    Assert::vulkan(
      vkCreateImageView(device, &createInfo, nullptr, &imageView),
      "Failed to create image view"
    );

    return imageView;
  }
}