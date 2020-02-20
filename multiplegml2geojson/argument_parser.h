#include <iostream>
#include <iterator>
#include <boost/program_options.hpp>
#include <exception>

// https://stackoverflow.com/questions/10194963/how-do-i-get-default-argument-values-with-boost-program-options :-)

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
namespace po = boost::program_options;

struct Config {
  std::string jsonLocation;
  std::vector<std::string> inputFiles;
  int nWorkers;
};

Config *parse_arguments(int ac, char** av) {
  Config *fullConfig = new Config();
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "print available arguments")
      ("config,c", po::value<std::string>()->default_value("config.json"), "json configuration file")
      ("files,f", po::value<std::vector<std::string>>()->multitoken(), "input xml files")
      ("workers,j", po::value<int>()->default_value(5), "amount of concurrent jobs");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      exit(0);
    } else if (vm.count("files") == 0) {
      cerr << "no input files provided\n";
      exit(1);
    }
    fullConfig->inputFiles = vm["files"].as<std::vector<std::string>>();
    fullConfig->jsonLocation = vm["config"].as<std::string>();
    fullConfig->nWorkers = vm["workers"].as<int>();
  }
  catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    exit(1);
  }
  catch(...) {
    cerr << "Exception of unknown type!\n";
    exit(1);
  }
  return fullConfig;
}
