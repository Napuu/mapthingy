#include "ogrsf_frmts.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
using namespace boost::filesystem;
int factorial(int number)
{
  int iteration, factorial=1;
  for(iteration=1; iteration<=number; iteration++)
  {
    factorial=factorial*iteration;
  }
  return factorial;
}

void printLayers(GDALDataset* poDS)
{
  for(auto layer: poDS->GetLayers()) {
    std::cout << layer->GetName() << std::endl;
  }
}

std::vector<std::string> printFiles() 
{
  path p("xml");
  std::vector<std::string> validFiles;
  if (is_directory(p)) {
    for (auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
      if (entry.path().string().find(".xml") != std::string::npos) {
        if (entry.path().string().find("L4") == std::string::npos) continue;
        validFiles.push_back(entry.path().string());
        //std::cout << entry << std::endl;
      }
    }
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
