const tippecanoe = require('tippecanoe');
const fs = require('fs');
const walk = require('walk');
const configurations = require('./configurations.js');
//console.log(test);

const walker = walk.walk("tiles", {});

walker.on("file", (root, stats, next)  => {
  if (stats.size < 1000000) {
    console.log(`size of "${stats.name}" under 1mb, maybe something is missing?`);
  }
  const noExtension = stats.name.split(".")[0];
  const tile = noExtension.split("_")[0];
  const layer = noExtension.split("_")[1];
  function handleTileTippecanoe(srcName) {
    const path = `./${root}/${stats.name}`;
    if (configurations[srcName] === undefined) {
      console.log(`skipping ${srcName}, no matching configuration found`);
    } else {
      configurations[srcName].forEach(configuration => {
        tippecanoe([path], { ...configuration, output: `${tile}_${configuration.layer}.mbtiles`}, {echo: true});
      });
    }
  }
  handleTileTippecanoe(layer);
  next();
});


