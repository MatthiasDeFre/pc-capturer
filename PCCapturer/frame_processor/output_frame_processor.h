#pragma once
#include "../sampler/sampler.h"
#include "../camera/camera.h"
#include "../multi_layer_processor.h"
class OutputFrameProcessor
{
	public:
		OutputFrameProcessor(std::shared_ptr<Camera> _cam, std::shared_ptr<Sampler> _sampler, std::shared_ptr<MultiLayerProcessor> _m_proc);
		int ProcessNextFrame();
	private:
		std::shared_ptr<Camera> cam;
		std::shared_ptr<Sampler> sampler;
		std::shared_ptr<MultiLayerProcessor> m_proc;
		int curr_frame = -1;
};

