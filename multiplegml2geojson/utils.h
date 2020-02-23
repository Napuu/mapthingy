#ifndef UTILS_H
#define UTILS_H
#include "ogrsf_frmts.h"
namespace utils{
int factorial(int number);
void printLayers(GDALDataset* poDS);
std::vector<std::string> printFiles(std::string directory, std::string prefix);
bool checkLayerExists(GDALDataset* poDS, std::string srcLayer); 
};
#endif
