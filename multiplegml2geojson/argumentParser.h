#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <iterator>
#include <boost/program_options.hpp>
#include <exception>

namespace argumentparser {
using std::cerr;
using std::cout;
using std::endl;
using std::exception;

struct ConfigurationParser {
  ConfigurationParser();
  public:
    void parseArguments(int, char*[]);
    void parseConfigurationFile();
    boost::property_tree::ptree tree;
    std::string jsonLocation;
    std::string outputFilename;
    std::string inputDirectory;
    std::string inputTilePrefix;
    int nWorkers;
};

}
#endif
