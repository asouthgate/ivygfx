#ifndef PIPELINE
#define PIPELINE

#include <string>
#include <vector>

#include "logical_device.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"


namespace ivy {

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
    
    class Pipeline {
        public:
            Pipeline(LogicalDevice& ld, SwapChain& swapChain, RenderPass& renderPass, 
                        const std::string& vertFilePath, 
                        const std::string& fragFilepath,
                        uint32_t w,
                        uint32_t h);

            ~Pipeline();

            Pipeline(const Pipeline&) = delete;
            void operator=(const Pipeline&) = delete;
        
            void populateDefaultPipelineConfigInfo(uint32_t w, uint32_t h);

            VkPipeline& getGraphicsPipelineHandle() { return graphicsPipeline; }

        private:
            static std::vector<char> readFile(const std::string& fpath);
            // void createRenderPass();        
            // void createGraphicsPipeline(LogicalDevice& logicalDevice,
            //                             const std::string& vertFilePath,
            //                             const std::string& fragFilepath,
            //                             int& w, int& h
            //                             );

            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            // we must require that any device will outlive our pipeline
            // see aggregation in UML
            LogicalDevice& logicalDevice;
            VkPipelineLayout pipelineLayout;
            VkPipeline graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
            PipelineConfigInfo configInfo;
    };

}

#endif