#include <iostream>
#include <vector>
namespace tileprocessor {

struct TileProcessingParameters {
  std::string outputDirectory;
  std::string outputFilename;
  std::vector<std::string> fields;
  std::string targetLayer;
  std::string geometryType;
  std::string inputDirectory;
  std::string inputTilePrefix;
};
void processTile(TileProcessingParameters params);

}
