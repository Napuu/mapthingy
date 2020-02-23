#include "argumentParser.h"
#include <boost/property_tree/json_parser.hpp>

namespace argumentparser {
ConfigurationParser::ConfigurationParser() {};
void ConfigurationParser::parseArguments(int ac, char *av[]) {
  try {
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "print available arguments")
      ("config,c", boost::program_options::value<std::string>()->default_value("config.json"), "json configuration file")
      ("input-directory,f", boost::program_options::value<std::string>(), "location of input xml files")
      ("input-tile-prefix,p", boost::program_options::value<std::string>()->default_value("*"), "prefix of tiles included (M4441* to select M4441L.xml, M44441R.xml etc.")
      ("workers,j", boost::program_options::value<int>()->default_value(5), "amount of concurrent jobs")
      ("output-name,o", boost::program_options::value<std::string>(), "name of outputfile");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(ac, av, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      exit(0);
    }

    bool err = false;
    if (vm.count("input-directory") == 0) {
      cerr << "no input directory provided\n";
      err = true;
    }
    if (vm.count("output-name") == 0) {
      cerr << "no name for outputfile provided\n";
      err = true;
    }
    if (err) exit(1);

    this->inputDirectory = vm["input-directory"].as<std::string>();
    this->inputTilePrefix = vm["input-tile-prefix"].as<std::string>();
    this->jsonLocation = vm["config"].as<std::string>();
    this->outputFilename = vm["output-name"].as<std::string>();
    this->nWorkers = vm["workers"].as<int>();
  }
  catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    exit(1);
  }
  catch(...) {
    cerr << "Exception of unknown type!\n";
    exit(1);
  }
}

void ConfigurationParser::parseConfigurationFile() {
  boost::property_tree::json_parser::read_json("config.json", this->tree);
}

}
