#include <librealsense2/rs.hpp>
#define WIN32_LEAN_AND_MEAN 

// PCL 
#include <pcl/common/io.h>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/console/parse.h>
#include <boost/thread/thread.hpp>
#include <filesystem>
#include <pcl/compression/octree_pointcloud_compression.h>
#include <chrono>
#include "sampler/sampler_uniform.h"
#include "encoder/encoder_draco.h"
#include "transporter/transporter_udp_proxy.h"
#include "multi_layer_processor.h"
//#include "WebRTCTransporter.h"
#include <Windows.h>
#include <winsock2.h>
//#include "WebRTCTransporter.h"
#include "result_writer.h"
#include "camera/camera_realsense.h"
#include "camera/camera_mock.h"
#include "frame_processor/output_frame_processor.h"
#pragma comment(lib,"ws2_32.lib") //Winsock Library

// Global Variables
std::string cloudFile; // .pcd file name
std::string prevCloudFile; // .pcd file name (Old cloud)
int i = 1; // Index for incremental file name

// Code to parse command line parameters
class InputParser {
public:
    InputParser(int& argc, char** argv) {
        for (int i = 1; i < argc; ++i)
            this->tokens.push_back(std::string(argv[i]));
    }
    /// @author iain
    const std::string& getCmdOption(const std::string& option) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }
    /// @author iain
    bool cmdOptionExists(const std::string& option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option)
            != this->tokens.end();
    }
private:
    std::vector <std::string> tokens;
};


//#define SAVE_RESULTS
int main(int argc, char** argv)
{
    InputParser input(argc, argv);

    std::shared_ptr<Transporter> transporter(nullptr);
    std::shared_ptr<Camera> cam(nullptr);

    // ------ Command line argument processing --------
    const std::string& results_file = input.getCmdOption("-r");
    bool save_results = false;
    std::cout << results_file << " file";
    if (!results_file.empty()) {
        ResultWriter::setFileName(results_file);
        std::cout << results_file;
    }
    ResultWriter::init();

    std::string proxy_addr = std::string(input.getCmdOption("-a"));
    if (proxy_addr.empty()) {
        std::cout << "Provide proxy address with -a";
        return 0;
    }

    std::string proxy_port_str = std::string(input.getCmdOption("-p"));
    uint32_t proxy_port;
    if (proxy_port_str.empty()) {
        std::cout << "Provide proxy port with -p";
        return 0;
    }
    else {
        proxy_port = std::stoi(proxy_port_str);
    }

    bool use_camera = false;
    std::string content_dir;
    if (input.cmdOptionExists("-i")) {
        use_camera = true;
    }
    else {
        content_dir = std::string(input.getCmdOption("-d"));
        if (content_dir.empty()) {
            std::cout << "Use -d to provide a directory with .ply files when not using a RealSense camera";
            return 0;
        }
    }
    // END ------ Command line argument processing --------
    std::vector<float> layer_ratios = { 0.60, 0.25 };
    std::shared_ptr<Sampler> sampler = std::make_shared<SamplerUniform>(SamplerUniform{ layer_ratios });


    // ------ Network tranport initiliasation ------
    std::cout << "transporter";
    transporter = std::make_shared<TransporterUDPProxy>(TransporterUDPProxy{});
    transporter->Init();
    transporter->SetupConnection(proxy_addr, proxy_port);
    while (!transporter->GetIsClientConnected()) {
        // Busy Wait, maybe change to condition variable
    }
    std::cout << "can start sending data" << std::endl;


    // END ------ Network tranport initiliasation ------
    // ------ Capturing & Processing Loop ------
    MultiLayerProcessor m(transporter);
    int curr_frame = -1;
    if (use_camera) {
        cam = std::make_shared<CameraRealsense>(CameraRealsense{  });
    }
    else {
        cam = std::make_shared<CameraMock>(CameraMock{ content_dir });
    }
    std::shared_ptr<MultiLayerProcessor> multi_layer_processor = std::make_shared<MultiLayerProcessor>(transporter);
    OutputFrameProcessor o_proc(cam, sampler, multi_layer_processor);
    while (true) {
        o_proc.ProcessNextFrame();
    }
    // END ------ Capturing & Processing Loop ------
    std::cout << "Exiting Program... " << std::endl;
    return EXIT_SUCCESS;
}
