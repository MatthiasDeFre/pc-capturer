#include "output_frame_processor.h"

OutputFrameProcessor::OutputFrameProcessor(std::shared_ptr<Camera> _cam, std::shared_ptr<Sampler> _sampler, std::shared_ptr<MultiLayerProcessor> _m_proc) : cam(_cam), sampler(_sampler), m_proc(_m_proc)
{
}

int OutputFrameProcessor::ProcessNextFrame()
{
    curr_frame++;
    // Capture a frame and save it in the cam object
    // Parameter => perform background removal
    cam->captureFrame(true);
    // Required for result logging
    sampler->setFrameId(curr_frame);
    // Set the current captured frame as the next frame to be sampled
    sampler->setCloud(cam->getCurrentFrame());
    // Perform sampling
    sampler->createLayers();
    //    pcl::io::savePLYFileASCII("frame_vop_" + std::to_string(curr_frame) +     ".ply", *cam->getCurrentFrame());
    std::cout << curr_frame;
    // Get bounding box coordinates of the captured object 
    // Can also include the position of the object here by adding the position to each vector4f
    Eigen::Vector4f min_p;
    Eigen::Vector4f max_p;
    pcl::getMinMax3D(*cam->getCurrentFrame(), min_p, max_p);
    pcl::PointXYZ min_pt(min_p[0], min_p[1], min_p[2]);
    pcl::PointXYZ max_pt(max_p[0], max_p[1], max_p[2]);
    // Multi layer processor will process the different layers by parallel encoding and transmitting them via the selected transporter
    m_proc->process_layers(min_pt, max_pt, std::move(sampler->getLayers()));
	return 0;
}
