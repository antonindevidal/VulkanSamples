#pragma once
#include "pch.h"


struct GraphicsPipeline
{
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;
};

struct ComputePipeline
{
	VkPipelineLayout _pipelineLayout;
	VkPipeline _pipeline;
};