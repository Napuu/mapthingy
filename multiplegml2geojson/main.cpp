#include <iostream>
#include <boost/array.hpp>
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include "utils.h"
#include "argumentParser.h"
#include "featureExtractor.h"

struct TileProcessingParameters {
  std::string outputFilename;
  std::vector<std::string> fields;
  std::string targetLayer;
  std::string geometryType;
  std::string inputDirectory;
  std::string inputTilePrefix;
};

void processTile(TileProcessingParameters params) {
  feature_extractor::FeatureExtractor *extractor = new feature_extractor::FeatureExtractor();
  std::vector<std::string> valid = utils::printFiles(params.inputDirectory, params.inputTilePrefix);
  extractor->CreateDstDS(params.outputFilename + "_" + params.targetLayer + ".geojson");
  extractor->CreateDstLayer(params.targetLayer, params.geometryType);

  for (auto & entry: valid) {
    extractor->AcquireSrcDS(entry);
    std::cout << entry << " " << params.targetLayer << std::endl;
    for (auto & field: params.fields) {
      extractor->LayerFromDst2Src(field);
    }
  }
  extractor->CloseDstDS();
}

int main(int argc, char *argv[]) {
  argumentparser::ConfigurationParser config;
  config.parseArguments(argc, argv);
  config.parseConfigurationFile();
  GDALAllRegister();

  boost::property_tree::ptree tree;
	
  boost::property_tree::json_parser::read_json("config.json", tree);
  std::vector<boost::thread> threads;
  boost::thread_group g;
  for (auto layer: tree) {
    std::string targetlayer = layer.first;
    std::vector<std::string> fields;
    std::string geometryType = layer.second.get_child("geometrytype").data();

    for (auto fieldname: layer.second.get_child("fields")) {
      fields.push_back(fieldname.second.data());
    }
    
    struct TileProcessingParameters params = {config.outputFilename, fields, targetlayer, geometryType, config.inputDirectory, config.inputTilePrefix};
    g.create_thread(boost::bind(processTile, params));
    //g.add_thread(&boost::create_thread(processTile, "M4", fields, targetlayer, geometryType));
    //processTile("M4", fields, targetlayer, geometryType);
	}
  g.join_all();
  return 0;
}

