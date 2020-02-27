#ifndef FEATUREEXTRACTOR_H 
#define FEATUREEXTRACTOR_H 

#include "ogrsf_frmts.h"

namespace feature_extractor {

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
  void CloseSrcDS();
};

}

#endif
