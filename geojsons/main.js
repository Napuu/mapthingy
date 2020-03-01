const tippecanoe = require('tippecanoe');
const fs = require('fs');
const walk = require('walk');
const configurations = require('./configurations.js');
const _exec = require('child_process').exec;
const util = require('util');
const exec = util.promisify(_exec);
const _ = require('underscore');

const walker = walk.walk("tiles", {});

walker.on("file", (root, stats, next)  => { // walker is not actually useful at all in this case :-) fs.readdir would be enough
  if (stats.size < 1000000) {
    console.log(`size of "${stats.name}" under 1mb, maybe something is missing?`);
  }

  const noExtension = stats.name.split(".")[0];
  const tile = noExtension.split("_")[0];
  const layer = noExtension.split("_")[1];

  function handleTileTippecanoe(srcName) {
    return; // remove this
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

async function handleTileJoin(groupedByLayer) {
  const keys = Object.keys(groupedByLayer);
  for (let i = 0; i < keys.length; i++) {
    const layerName = keys[i];
    const files = groupedByLayer[layerName];
    const cmd = `tile-join -pk -f -o ${layerName} ${files.join(' ')}`;
    console.log(cmd);
    //await exec(cmd);
  }
}

walker.on("end", () => {
  console.log("tippecanoe done, now joining tiles");
  const files = fs.readdirSync(".");
  const mbtilesFiles = files.filter(fileName => fileName.endsWith("mbtiles") && fileName.indexOf("_") !== -1);
  const groupedByLayer = _.groupBy(mbtilesFiles, (file) => {
    return file.split("_")[1];
  });
  console.log(groupedByLayer);
  handleTileJoin(groupedByLayer);
});

