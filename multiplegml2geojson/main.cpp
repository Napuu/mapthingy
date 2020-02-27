#include <iostream>
#include <boost/array.hpp>
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include "utils.h"
#include "argumentParser.h"
#include "featureExtractor.h"
#include "tileProcessor.h"


int main(int argc, char *argv[]) {
  argumentparser::ConfigurationParser config;
  config.parseArguments(argc, argv);
  config.parseConfigurationFile();

  boost::property_tree::ptree tree;
	
  boost::property_tree::json_parser::read_json("config.json", tree);
  std::vector<boost::thread> threads;
  boost::thread_group g;
  GDALAllRegister();
  for (auto layer: tree) {
    std::string targetlayer = layer.first;
    std::vector<std::string> fields;
    std::string geometryType = layer.second.get_child("geometrytype").data();

    for (auto fieldname: layer.second.get_child("fields")) {
      fields.push_back(fieldname.second.data());
    }
    
    struct tileprocessor::TileProcessingParameters params = {config.outputDirectory, config.outputFilename, fields, targetlayer, geometryType, config.inputDirectory, config.inputTilePrefix};
    //tileprocessor::processTile(params);
    g.create_thread(boost::bind(tileprocessor::processTile, params));
    //g.add_thread(&boost::create_thread(processTile, "M4", fields, targetlayer, geometryType));
    //processTile("M4", fields, targetlayer, geometryType);
	}
  g.join_all();
  return 0;
}

