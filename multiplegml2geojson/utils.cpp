#include "utils.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

namespace utils {
void printLayers(GDALDataset* poDS) {
  for(auto layer: poDS->GetLayers()) {
    std::cout << layer->GetName() << std::endl;
  }
}

std::vector<std::string> printFiles(std::string directory, std::string prefix) {
  boost::filesystem::path p(directory);
  std::vector<std::string> validFiles;
  if (is_directory(p)) {
    for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(p), {})) {
      if (entry.path().string().find(".xml") != std::string::npos) {
        if (entry.path().string().find(prefix) == std::string::npos) continue;
        validFiles.push_back(entry.path().string());
      }
    }
  } else {
    std::cerr << "\"" << directory << "\" is not a directory\n";
    std::terminate();
  }
  return validFiles;
}

bool checkLayerExists(GDALDataset* poDS, std::string srcLayer) 
{
  for (auto layer: poDS->GetLayers()) {
    if (layer->GetName() == srcLayer) {
      return true;
    }
  }
  return false;
}

};
