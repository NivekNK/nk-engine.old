#include "nkpch.h"

#include "renderer/device.h"

namespace nk {
    Device::Device(Platform& platform, Instance& instance, VkAllocationCallbacks* allocator)
        : m_instance{instance},
          m_allocator{allocator} {
        m_swapchain_support_info_allocator = new MallocAllocator("SwapchainSupportInfo", MemoryType::Renderer);

        m_surface = platform.create_surface(instance, allocator);
        InfoLog("Vulkan surface created.");

        MallocAllocator defer{"Defer", MemoryType::Defer};
        select_physical_device(&defer);
        detect_depth_format();
        create_logical_device();
        create_command_pool();
        TraceLog("nk::Device created.");
    }

    Device::~Device() {
        vkDestroyCommandPool(m_logical_device, m_graphics_command_pool, m_allocator);
        InfoLog("Vulkan Graphics Command Pool destroyed.");

        vkDestroyDevice(m_logical_device, m_allocator);
        InfoLog("Vulkan Logical Device destroyed.");

        m_swapchain_support_info.formats.free(m_swapchain_support_info_allocator);
        m_swapchain_support_info.present_modes.free(m_swapchain_support_info_allocator);
        delete m_swapchain_support_info_allocator;

        vkDestroySurfaceKHR(m_instance(), m_surface, m_allocator);
        InfoLog("Vulkan Surface destroyed.");

        TraceLog("nk::Device destroyed.");
    }

    bool Device::find_memory_index(u32& out_memory_index, const u32 type_filter, VkMemoryPropertyFlags property_flags) const {
        VkPhysicalDeviceMemoryProperties memory_properties;
        vkGetPhysicalDeviceMemoryProperties(m_physical_device, &memory_properties);

        for (u32 i = 0; i < memory_properties.memoryTypeCount; ++i) {
            // Check each memory type to see if its bit is set to 1.
            if (type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags) {
                out_memory_index = i;
                return true;
            }
        }

        WarnLog("Unable to find suitable memory type!");
        out_memory_index = u32_max;
        return false;
    }

    void Device::select_physical_device(Allocator* defer) {
        u32 physical_device_count = 0;
        VulkanCheck(vkEnumeratePhysicalDevices(m_instance(), &physical_device_count, 0));
        if (physical_device_count == 0) {
            FatalLog("No devices which support Vulkan were found.");
            return;
        }
        VkPhysicalDevice* physical_devices = defer->allocate<VkPhysicalDevice>(physical_device_count);
        VulkanCheck(vkEnumeratePhysicalDevices(m_instance(), &physical_device_count, physical_devices));

        // TODO: These requirements should probably be driven by engine
        // configuration.
        auto requirements = PhysicalDeviceRequirements{
            .graphics = true,
            .present = true,
            .transfer = true,
            // NOTE: Enable this if compute will be required.
            // .compute = true;
            .sampler_anisotropy = true,
            .discrete_gpu = true,
            .extensions = {defer, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}}};

        for (i32 i = 0; i < physical_device_count; i++) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

            VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

            VkPhysicalDeviceMemoryProperties memory;
            vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

            auto queue_family = PhysicalDeviceQueueFamilyInfo{};
            bool meet_requirements = physical_device_meets_requirements(
                &queue_family,
                physical_devices[i],
                properties,
                features,
                requirements,
                defer);

            if (!meet_requirements) {
                continue;
            }

#if defined(NK_DEBUG)
            DebugLog("Selected device: '{}'.", properties.deviceName);
            // GPU type, etc.
            switch (properties.deviceType) {
                default:
                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    DebugLog("GPU type is Unknown.");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    DebugLog("GPU type is Integrated.");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    DebugLog("GPU type is Discrete.");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    DebugLog("GPU type is Virtual.");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    DebugLog("GPU type is CPU.");
                    break;
            }

            DebugLog(
                "GPU Driver version: {}.{}.{}.",
                VK_VERSION_MAJOR(properties.driverVersion),
                VK_VERSION_MINOR(properties.driverVersion),
                VK_VERSION_PATCH(properties.driverVersion));

            // Vulkan API version.
            DebugLog(
                "Vulkan API version: {}.{}.{}.",
                VK_VERSION_MAJOR(properties.apiVersion),
                VK_VERSION_MINOR(properties.apiVersion),
                VK_VERSION_PATCH(properties.apiVersion));

            // Memory information
            for (u32 i = 0; i < memory.memoryHeapCount; ++i) {
                const f32 memory_size_gib = static_cast<f32>(memory.memoryHeaps[i].size) / 1024.0f / 1024.0f / 1024.0f;
                if (memory.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                    DebugLog("[Memory Heap {}] Local GPU memory: {:.2f} GiB.", i, memory_size_gib);
                } else {
                    DebugLog("[Memory Heap {}] Shared System memory: {:.2f} GiB.", i, memory_size_gib);
                }
            }
#endif

            m_physical_device = physical_devices[i];
            m_queue_family = queue_family;

            // Keep a copy of properties, features and memory info for later use.
            m_properties = properties;
            m_features = features;
            m_memory = memory;
        }
        requirements.extensions.clear();
        defer->free(physical_devices, sizeof(VkPhysicalDevice) * physical_device_count);

        // Ensure a device was selected
        if (!m_physical_device) {
            ErrorLog("No physical devices were found which meet the requirements.");
            return;
        }

        InfoLog("Physical device selected.");
    }

    void Device::detect_depth_format() {
        // Format candidates
        constexpr u64 candidate_count = 3;
        VkFormat candidates[candidate_count] = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };

        u32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
        for (u64 i = 0; i < candidate_count; ++i) {
            VkFormatProperties properties;
            vkGetPhysicalDeviceFormatProperties(m_physical_device, candidates[i], &properties);

            if ((properties.linearTilingFeatures & flags) == flags) {
                m_depth_format = candidates[i];
                InfoLog("Depth format detected.");
                return;
            } else if ((properties.optimalTilingFeatures & flags) == flags) {
                m_depth_format = candidates[i];
                InfoLog("Depth format detected.");
                return;
            }
        }

        m_depth_format = VK_FORMAT_UNDEFINED;
        WarnLog("Depth format not detected.");
    }

    void Device::create_logical_device() {
        // NOTE: Do not create additional queues for shared indices.
        constexpr u32 unique_queue_family_count = 3;
        u32 unique_queue_families[unique_queue_family_count] = {
            m_queue_family.graphics_family_index,
            m_queue_family.present_family_index,
            m_queue_family.transfer_family_index
        };

        VkDeviceQueueCreateInfo queue_create_infos[unique_queue_family_count] = {};
        u32 i = 0;
        for (const u32 queue_family_index : unique_queue_families) {
            queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_infos[i].queueFamilyIndex = queue_family_index;
            queue_create_infos[i].queueCount = 1;

            // TODO: Enable this for a future enhacement.
            // if (indices[i] == m_queue_family.graphics_family_index)
            //    queue_create_infos[i].queueCount = 2;

            queue_create_infos[i].flags = 0;
            queue_create_infos[i].pNext = nullptr;

            f32 queue_priority = 1.0f;
            queue_create_infos[i].pQueuePriorities = &queue_priority;
            ++i;
        }

        // Request device features.
        // TODO: should be configurable.
        VkPhysicalDeviceFeatures device_features = {};
        device_features.samplerAnisotropy = VK_TRUE; // Request anistrophy

        VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
        device_create_info.queueCreateInfoCount = unique_queue_family_count;
        device_create_info.pQueueCreateInfos = queue_create_infos;
        device_create_info.pEnabledFeatures = &device_features;
        device_create_info.enabledExtensionCount = 1;
        const char* extension_names = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        device_create_info.ppEnabledExtensionNames = &extension_names;

        // Deprecated and ignored, so pass nothing.
        device_create_info.enabledLayerCount = 0;
        device_create_info.ppEnabledLayerNames = nullptr;

        // Create the device.
        VulkanCheck(vkCreateDevice(m_physical_device, &device_create_info, m_allocator, &m_logical_device));
        InfoLog("Vulkan Logical Device created.");

        // Get queues.
        vkGetDeviceQueue(m_logical_device, m_queue_family.graphics_family_index, 0, &m_graphics_queue);
        vkGetDeviceQueue(m_logical_device, m_queue_family.present_family_index, 0, &m_present_queue);
        vkGetDeviceQueue(m_logical_device, m_queue_family.transfer_family_index, 0, &m_transfer_queue);
        InfoLog("Vulkan queues obtained.");
    }

    void Device::create_command_pool() {
        VkCommandPoolCreateInfo pool_create_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        pool_create_info.queueFamilyIndex = m_queue_family.graphics_family_index;
        pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VulkanCheck(vkCreateCommandPool(m_logical_device, &pool_create_info, m_allocator, &m_graphics_command_pool));
        InfoLog("Vulkan Graphics Command Pool created.");
    }

    void Device::query_swapchain_support(VkPhysicalDevice physical_device) {
        // Surface capabilities
        VulkanCheck(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, m_surface, &m_swapchain_support_info.capabilities));

        // Surface formats
        u32 format_count = 0;
        VulkanCheck(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_surface, &format_count, nullptr));
        if (format_count != 0) {
            VkSurfaceFormatKHR* formats = m_swapchain_support_info_allocator->allocate<VkSurfaceFormatKHR>(format_count);
            m_swapchain_support_info.formats.init(formats, format_count);
            VulkanCheck(vkGetPhysicalDeviceSurfaceFormatsKHR(
                physical_device,
                m_surface,
                &format_count,
                m_swapchain_support_info.formats.data()));
        }

        // Present modes
        u32 present_mode_count = 0;
        VulkanCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, m_surface, &present_mode_count, nullptr));
        if (present_mode_count != 0) {
            VkPresentModeKHR* present_modes = m_swapchain_support_info_allocator->allocate<VkPresentModeKHR>(present_mode_count);
            m_swapchain_support_info.present_modes.init(present_modes, present_mode_count);
            VulkanCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(
                physical_device,
                m_surface,
                &present_mode_count,
                m_swapchain_support_info.present_modes.data()));
        }
    }

    bool Device::physical_device_meets_requirements(
        PhysicalDeviceQueueFamilyInfo* out_queue_family,
        VkPhysicalDevice physical_device,
        const VkPhysicalDeviceProperties& properties,
        const VkPhysicalDeviceFeatures& features,
        const PhysicalDeviceRequirements& requirements,
        Allocator* defer) {
        // Evaluate device properties to determine if it meets the needs of our application.
        out_queue_family->graphics_family_index = -1;
        out_queue_family->present_family_index = -1;
        out_queue_family->compute_family_index = -1;
        out_queue_family->transfer_family_index = -1;

        // Check if discrete GPU
        if (requirements.discrete_gpu) {
            if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                DebugLog("Device {} is not a discrete GPU, and one is required. Skipping.", properties.deviceName);
                return false;
            }
        }

        // Sampler anisotropy
        if (requirements.sampler_anisotropy && !features.samplerAnisotropy) {
            DebugLog("Device {} does not support samplerAnisotropy, skipping.", properties.deviceName);
            return false;
        }

        u32 queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
        VkQueueFamilyProperties* queue_families = defer->allocate<VkQueueFamilyProperties>(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

        // Look at each queue and see what queues it supports
        u8 min_transfer_score = 255;
        for (u32 i = 0; i < queue_family_count; i++) {
            u8 current_transfer_score = 0;

            // Graphics queue
            if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                out_queue_family->graphics_family_index = i;
                current_transfer_score++;
            }

            // Compute queue
            if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                out_queue_family->compute_family_index = i;
                current_transfer_score++;
            }

            // Transfer queue
            if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
                // Take the index if it is the current lowest. This increases the
                // liklihood that it is a dedicated transfer queue.
                if (current_transfer_score <= min_transfer_score) {
                    min_transfer_score = current_transfer_score;
                    out_queue_family->transfer_family_index = i;
                }
            }

            // Present queue
            VkBool32 supports_present = VK_FALSE;
            VulkanCheck(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, m_surface, &supports_present));
            if (supports_present) {
                out_queue_family->present_family_index = i;
            }
        }
        defer->free(queue_families, sizeof(VkQueueFamilyProperties) * queue_family_count);

        // Print out some info about the device
        DebugLog("Device selected:\n{:>12} | {:>12} | {:>12} | {:>12} | {:>12}\n{:>12} | {:>12} | {:>12} | {:>12} | {:>12}",
                 "Graphics", "Present", "Compute", "Transfer", "Name",
                 out_queue_family->graphics_family_index != -1,
                 out_queue_family->present_family_index != -1,
                 out_queue_family->compute_family_index != -1,
                 out_queue_family->transfer_family_index != -1,
                 properties.deviceName);

        if ((requirements.graphics && out_queue_family->graphics_family_index == -1) ||
            (requirements.present && out_queue_family->present_family_index == -1) ||
            (requirements.compute && out_queue_family->compute_family_index == -1) ||
            (requirements.transfer && out_queue_family->transfer_family_index == -1)) {
            return false;
        }

        DebugLog("Device meets queue requirements.");
        DebugLog("Graphics Family Index: {}.", out_queue_family->graphics_family_index);
        DebugLog("Present Family Index:  {}.", out_queue_family->present_family_index);
        DebugLog("Transfer Family Index: {}.", out_queue_family->transfer_family_index);
        DebugLog("Compute Family Index:  {}.", out_queue_family->compute_family_index);

        query_swapchain_support(physical_device);
        if (m_swapchain_support_info.formats.empty() || m_swapchain_support_info.present_modes.empty()) {
            m_swapchain_support_info.formats.free(m_swapchain_support_info_allocator);
            m_swapchain_support_info.present_modes.free(m_swapchain_support_info_allocator);
            DebugLog("Required swapchain support not present, skipping device {}.", properties.deviceName);
            return false;
        }

        u32 available_extension_count = 0;
        VulkanCheck(vkEnumerateDeviceExtensionProperties(
            physical_device, nullptr, &available_extension_count, nullptr));

        VkExtensionProperties* available_extensions_properties
            = defer->allocate<VkExtensionProperties>(available_extension_count);
        VulkanCheck(vkEnumerateDeviceExtensionProperties(
            physical_device, nullptr, &available_extension_count, available_extensions_properties));

        for (const auto& required_extension : requirements.extensions) {
            bool found_extension = false;

            for (u32 i = 0; i < available_extension_count; i++) {
                if (strcmp(available_extensions_properties[i].extensionName, required_extension) == 0) {
                    found_extension = true;
                    break;
                }
            }

            if (!found_extension) {
                DebugLog("Required extension not found: '{}', skipping device {}.", required_extension, properties.deviceName);
                defer->free(available_extensions_properties, sizeof(VkExtensionProperties) * available_extension_count);
                m_swapchain_support_info.formats.free(m_swapchain_support_info_allocator);
                m_swapchain_support_info.present_modes.free(m_swapchain_support_info_allocator);
                return false;
            }
        }
        defer->free(available_extensions_properties, sizeof(VkExtensionProperties) * available_extension_count);

        return true;
    }
}
