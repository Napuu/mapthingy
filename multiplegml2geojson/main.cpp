#include <iostream>
#include <boost/array.hpp>
#include <ogr_spatialref.h>
#include <ogrsf_frmts.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include "ogr_spatialref.h"
#include <boost/thread.hpp>
#include "utils.h"

using namespace boost::property_tree;
using namespace std;

class FeatureExtractor {
  GDALDataset *dstDS;
  GDALDataset *srcDS;
  OGRSpatialReference srcSRS, dstSRS;
  OGRLayer *dstLayer;
  OGRCoordinateTransformation *poCT;

  GDALDriver * GetDstDriver();
  public:
  FeatureExtractor();
  void LayerFromDst2Src(std::string);
  void CreateDstDS(std::string);
  void AcquireSrcDS(std::string);
  void CreateDstLayer(std::string, std::string);
  void InitializeSRS();
  void CloseDstDS();
};

FeatureExtractor::FeatureExtractor () {
  InitializeSRS();
}

void FeatureExtractor::CloseDstDS() {
  GDALClose(dstDS);
}

void FeatureExtractor::InitializeSRS () {
  srcSRS.importFromEPSG(3067);
  dstSRS.importFromEPSG(4326);
  poCT = OGRCreateCoordinateTransformation(&srcSRS, &dstSRS);
}

GDALDriver* FeatureExtractor::GetDstDriver() {
  const char *pszDriverName = "GeoJSON";
  GDALDriver *dstDriver;
  dstDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
  if (dstDriver == NULL) {
    printf("error creating dstDriver");
  }
  return dstDriver;
}

void FeatureExtractor::CreateDstDS(std::string dstDSName) {
  GDALDriver *poDriver = GetDstDriver();
  dstDS = poDriver->Create(dstDSName.c_str(), 0, 0, 0, GDT_Unknown, NULL);
}

void FeatureExtractor::AcquireSrcDS(std::string srcDSName) {
  srcDS = (GDALDataset*) GDALOpenEx(srcDSName.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL );
  if( srcDS == NULL )
  {
    printf("Open failed.\n");
    exit(1);
  }
}

void FeatureExtractor::CreateDstLayer(std::string dstLayerName, std::string dstLayerType) {
  OGRwkbGeometryType geomType;
  if (dstLayerType == "polygon") {
    geomType = wkbPolygon;
  } else if (dstLayerType == "linestring") {
    geomType = wkbLineString;
  } else if (dstLayerType == "point") {
    geomType = wkbPoint;
  }
  dstLayer = dstDS->CreateLayer(dstLayerName.c_str(), &dstSRS, wkbPolygon, NULL );
}

void FeatureExtractor::LayerFromDst2Src(std::string srcLayerName) {
  if (!checkLayerExists(srcDS, srcLayerName)) {
    return;
  }
  OGRLayer* srcLayer = srcDS->GetLayerByName(srcLayerName.c_str());
  OGRFeature *poFeature;
  srcLayer->ResetReading();
  while( (poFeature = srcLayer->GetNextFeature()) != NULL )
  {
    poFeature->GetGeometryRef()->transform(poCT);
    poFeature->GetGeometryRef()->flattenTo2D();
    poFeature->GetGeometryRef()->swapXY();
    OGRErr err = dstLayer->CreateFeature(poFeature);
    if (err != 0) {
      printf("error creating feature");
      exit(1);
    }
  }

}

void processTile(std::string tile, std::vector<std::string> fields, std::string targetLayer, std::string geometryType) {
  std::cout << "ajlködsfjlk2" << std::endl;
  std::cout << geometryType << std::endl;
  FeatureExtractor *extractor = new FeatureExtractor();
  std::cout << "hello1" << std::endl;
  std::vector<std::string> valid = printFiles();
  std::cout << "hello2" << std::endl;
  std::cout << "first valid entry" << valid[0] << std::endl;
  extractor->CreateDstDS(tile + "_" + targetLayer + ".geojson");
  extractor->CreateDstLayer(targetLayer, geometryType);

  for (auto & entry: valid) {
    extractor->AcquireSrcDS(entry);
    std::cout << entry << " " << targetLayer << std::endl;
    for (auto & field: fields) {
      extractor->LayerFromDst2Src(field);
    }
  }
  extractor->CloseDstDS();
}

int main(int argc, char*argv[]) {
  /*

  extractor->CreateDstDS("M4_maankaytto.geojson");
  extractor->CreateDstLayer("maankaytto"); //needs geometry type
  std::vector<std::string> maankayttoFields;
  for (auto & entry: valid) {
    std::cout << entry << std::endl;
    extractor->AcquireSrcDS(entry);
    extractor->LayerFromDst2Src("TaajaanRakennettuAlue");
    extractor->LayerFromDst2Src("Maatalousmaa");
    extractor->LayerFromDst2Src("Luonnonsuojelualue");
    extractor->LayerFromDst2Src("Autoliikennealue");
    extractor->LayerFromDst2Src("Hautausmaa");
    extractor->LayerFromDst2Src("Hietikko");
    extractor->LayerFromDst2Src("Kaatopaikka");
  }

  extractor->CloseDstDS();
  */
	
  GDALAllRegister();

	ptree tree;
	
	json_parser::read_json("config2.json", tree);
  std::vector<boost::thread> threads;
  boost::thread_group g;
  for (auto layer: tree) {
    std::string targetlayer = layer.first;
    std::vector<std::string> fields;
    std::string geometryType = layer.second.get_child("geometrytype").data();
    for (auto fieldname: layer.second.get_child("fields")) {
      fields.push_back(fieldname.second.data());
    }
    
    g.create_thread(boost::bind(processTile, "M4", fields, targetlayer, geometryType));
    //g.add_thread(&boost::create_thread(processTile, "M4", fields, targetlayer, geometryType));
    //processTile("M4", fields, targetlayer, geometryType);
	}
  g.join_all();
  return 0;
}

