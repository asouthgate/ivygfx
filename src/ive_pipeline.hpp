#pragma once

#include "ive_device.hpp"

#include <string>
#include <vector>

namespace ive {

    // Config should be shareable between multiple pipelines
    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    
    class ivePipeline {
        public:
            ivePipeline(iveDevice& device, 
                        const std::string& vertFilePath, 
                        const std::string& fragFilepath,
                        const PipelineConfigInfo& configInfo);

            ~ivePipeline();

            ivePipeline(const ivePipeline&) = delete;
            void operator=(const ivePipeline&) = delete;
        
            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t w, uint32_t h);


        private:
            static std::vector<char> readFile(const std::string& fpath);

            void createGraphicsPipeline(iveDevice& device,
                                        const std::string& vertFilePath,
                                        const std::string& fragFilepath,
                                        const PipelineConfigInfo& configInfo
                                        );

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            // we must require that any device will outlive our pipeline
            // see aggregation in UML
            iveDevice& ive_device; 
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };

}
