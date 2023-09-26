#include "sampler_uniform.h"
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

SamplerUniform::SamplerUniform(std::vector<float> _layer_ratios) : Sampler() {
    layer_ratios = _layer_ratios;
    number_of_layers = layer_ratios.size() + 1;
}
SamplerUniform::SamplerUniform(std::string _file_name, std::vector<float> _layer_ratios) : Sampler(_file_name) {
    layer_ratios = _layer_ratios;
    number_of_layers = layer_ratios.size() + 1;
}
std::string SamplerUniform::getSamplerName() {
    std::string name = "uniform_sampler_" + std::to_string(number_of_layers) + "_layers_ratios_";
    for (auto x : layer_ratios) {
        name += std::to_string(x) + "_";
    }
    return name;
}
// Currently hardcoded for 3 layers
void SamplerUniform::createLayers() {
    auto start_clock = std::chrono::high_resolution_clock::now();
    layers.clear();
    std::vector<int> pc_points(input_cloud->size());
    std::iota(std::begin(pc_points), std::end(pc_points), 0); // Fill with 0, 1, ..., size -1.
    std::shuffle(pc_points.begin(), pc_points.end(), std::mt19937{std::random_device{}()});

    std::vector<int> layer_indices0;
    pcl::PointIndices::Ptr layer_point_indices0(new pcl::PointIndices);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr layer0(new pcl::PointCloud<pcl::PointXYZRGB>);
    std::vector<int> layer_indices1;
    pcl::PointIndices::Ptr layer_point_indices1(new pcl::PointIndices);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr layer1(new pcl::PointCloud<pcl::PointXYZRGB>);
    std::vector<int> layer_indices2;
    pcl::PointIndices::Ptr layer_point_indices2(new pcl::PointIndices);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr layer2(new pcl::PointCloud<pcl::PointXYZRGB>);

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr layer(new pcl::PointCloud<pcl::PointXYZRGB>);
    int size1 = input_cloud->size() * layer_ratios[0];
    int size2 = input_cloud->size() * layer_ratios[1];
    int size3 = input_cloud->size() - size1 - size2;
    int e1 = size1;
    int e2 = e1 + size2;
    layer0->reserve(size1);
    layer1->reserve(size2);
    layer2->reserve(size3);
    int p0 = 0;
    int p1 = 0;
    int p2 = 0;
    for (int i = 0; i < pc_points.size(); i++) {
        if (pc_points[i] > 0 && pc_points[i] < e1) {
            layer0->push_back(input_cloud->points[i]);
            p0++;
        }
        else if (pc_points[i] >= e1
            && pc_points[i] < e2) {
            layer1->push_back(input_cloud->points[i]);
            p1++;
        }
        else {
            layer2->push_back(input_cloud->points[i]);
            p2++;
        }
    }
    layers.push_back(layer0);
    layers.push_back(layer1);
    layers.push_back(layer2);
    auto end_clock = std::chrono::high_resolution_clock::now();
    int duration = duration_cast<std::chrono::milliseconds>(end_clock - start_clock).count();
    //ResultWriter::setLayersPerFrame(frame_id, 3);
    //ResultWriter::addSamplingDuration(frame_id, duration);
}