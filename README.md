# mapthingy
Experimenting with GDAL C++ api and basemap rendering

Datasets from National Land Survey of Finland (+ Digiroad, same geometries than Topographic Database but with more metadata) and Corine.


### Live demo https://napuu.xyz/mapboxfront/
Keep in mind that<br>
a) tiles are huge right now, rasters should be rendered at the backend / make vector tiles smaller<br>
b) stylesheet is very simple and missing lots of details. just trying out maputnik

Below a picture showing how zipped NLS's GML files are converted to ready to use vector tiles.
![architecture](BAF40233-E724-4330-8C61-AC5A068246DB.jpeg?raw=true "architecture")

