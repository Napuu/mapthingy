const fs = require('fs');
const util = require('util');
const readFile = util.promisify(fs.readFile);

async function main() {
  let filu = await readFile('style3.json')
  let jsonFilu = JSON.parse(filu.toString());
  let target = jsonFilu;
  const pintaLayers = getIndividualPintaLayers(target);
  let pintaStartingIndex = target.layers.findIndex(layer => layer.id.indexOf("pinta") == 0);
  const ogpsi = pintaStartingIndex;
  for (i in pintaLayers) {
    let alikulku = modifyLayer(cloneJson(pintaLayers[i]), -1);
    //console.log(alikulku);
    target.layers.splice(pintaStartingIndex, 0, alikulku);
    pintaStartingIndex += 1;
    //let tunneli = modifyLayer(cloneJson(pintaLayers[i]), -2);
    //target.layers.splice(pintaStartingIndex, 0, tunneli);


    //let ylikulku = modifyLayer(cloneJson(pintaLayers[i]), 1);
    //let silta = modifyLayer(cloneJson(pintaLayers[i]), 2);
  }
  pintaStartingIndex = ogpsi;
  for (i in pintaLayers) {
    let tunneli = modifyLayer(cloneJson(pintaLayers[i]), -2);
    target.layers.splice(pintaStartingIndex, 0, tunneli);
    pintaStartingIndex += 1;
  }
  let pintaEndingIndex = pintaStartingIndex;
  for (let i = pintaEndingIndex; i < target.layers.length; i++) {
    if (target.layers[i].id.indexOf("pinta") == -1 && target.layers[i].id.indexOf("tunneli") == -1 && target.layers[i].id.indexOf("alikulku") == -1) {
      break;
    } else {
      pintaEndingIndex += 1;
    }
  }
  for (i in pintaLayers) {
    let ylikulku = modifyLayer(cloneJson(pintaLayers[i]), 1);
    target.layers.splice(pintaEndingIndex, 0, ylikulku);
    pintaEndingIndex += 1;
  }
  for (i in pintaLayers) {
    let silta= modifyLayer(cloneJson(pintaLayers[i]), 2);
    target.layers.splice(pintaEndingIndex, 0, silta);
    pintaEndingIndex += 1;
  }
  console.log(target);
  fs.writeFileSync("out.json", JSON.stringify(target));
}


function cloneJson(json) {
  return JSON.parse(JSON.stringify(json));
}
function getIndividualPintaLayers(target) {
  let layerObjects = [];
  for (field in target.layers) {
    let layerObject = target.layers[field];
    if (layerObject.id.indexOf('pinta') == 0) {
      layerObjects.push(layerObject);
    }
  }
  layerObjects.splice
  return layerObjects;
}

function modifyLayer(target, level) {
  let replacement = "";
  switch (level) {
    case -1:
      replacement = "alikulku";
      break;
    case -2:
      replacement = "tunneli";
      break;
    case 1:
      replacement = "ylikulku";
      break;
    case 2:
      replacement = "silta";
      break;
  }
  target = modifyLayerName(target, replacement);
  target = modifyFilter(target, level);
  return target;
}

function modifyFilter(target, level) {
  for (i in target.filter) {
    if (typeof target.filter[i] == "string")  continue;
    let currentFilter = target.filter[i];
    if (currentFilter[1] == "SILTA_ALIK" || currentFilter[1] == "tasosijainti") {
      switch (level) {
        case -1:
          currentFilter[2] = -1;
          break;
        case -2:
          currentFilter[0] = "<";
          currentFilter[2] = -1;
          break;
        case 1:
          currentFilter[2] = 1;
          break;
        case 2:
          currentFilter[0] = ">";
          currentFilter[2] = 1;
          break;
      }
    }
  }
  return target;
}

function modifyLayerName(target, name) {
  target.id = target.id.replace("pinta", name);
  return target;
}

main();
