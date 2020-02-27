#include "tileProcessor.h"
#include "featureExtractor.h"
#include "utils.h"

namespace tileprocessor {

void processTile(TileProcessingParameters params) {
  feature_extractor::FeatureExtractor *extractor = new feature_extractor::FeatureExtractor();
  std::vector<std::string> valid = utils::printFiles(params.inputDirectory, params.inputTilePrefix);
  extractor->CreateDstDS(params.outputDirectory + params.outputFilename + "_" + params.targetLayer + ".geojson");
  extractor->CreateDstLayer(params.targetLayer, params.geometryType);

  for (auto & entry: valid) {
    std::cout << entry << " " << params.targetLayer << std::endl;
    extractor->AcquireSrcDS(entry);
    for (auto & field: params.fields) {
      extractor->LayerFromDst2Src(field);
    }
  }
  extractor->CloseDstDS();
}

}
