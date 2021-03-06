const fs = require('fs');
const util = require('util');
const readFile = util.promisify(fs.readFile);

const main = async (inputfilepath, outputfilepath) => {
  if (!inputfilepath || !outputfilepath) {
    console.error("inputfilepath and/or outputfilepath missing\nexiting");
    return;
  }
  const inputFile = await readFile(inputfilepath);
  const inputJSON = JSON.parse(inputFile.toString());

  addNewLayersToStylesheetJSON(inputJSON);

  fs.writeFileSync(outputfilepath, JSON.stringify(inputJSON));
};

function addNewLayersToStylesheetJSON (inputJSON) {
  const surfaceLayers = getIndividualSurfaceLayers(inputJSON);
  let layerInsertionIndex = inputJSON.layers.findIndex(layer => layer.id.indexOf('pinta') === 0);
  for (let level = -3; level <= 3; level++) {
    if (level === 0) {
      layerInsertionIndex += 1;
      for (let i = layerInsertionIndex; i < inputJSON.layers.length; i++) {
        if (inputJSON.layers[i].id.indexOf('pinta') === -1) {
          break;
        } else {
          layerInsertionIndex += 1;
        }
      }
    } else {
      for (const i in surfaceLayers) {
        const modifiedLayer = modifyLayer(cloneJson(surfaceLayers[i]), level);
        inputJSON.layers.splice(layerInsertionIndex, 0, modifiedLayer);
        layerInsertionIndex += 1;
      }
    }
  }
}

function getIndividualSurfaceLayers (inputJSON) {
  const layerObjects = [];
  for (const field in inputJSON.layers) {
    const layerObject = inputJSON.layers[field];
    if (layerObject.id.indexOf('pinta') === 0) {
      layerObjects.push(layerObject);
    }
  }
  return layerObjects;
}

function modifyLayer (inputJSON, level) {
  inputJSON = modifyLayerName(inputJSON, level);
  inputJSON = modifyFilter(inputJSON, level);
  return inputJSON;
}

function modifyLayerName (inputJSON, level) {
  const replacement = layerProperties[level].layerName;
  inputJSON.id = inputJSON.id.replace('pinta', replacement);
  return inputJSON;
}

function modifyFilter (inputJSON, level) {
  for (const i in inputJSON.filter) {
    if (typeof inputJSON.filter[i] === 'string') continue;
    const currentFilter = inputJSON.filter[i];
    if (currentFilter[1] === 'SILTA_ALIK' || currentFilter[1] === 'tasosijainti') {
      currentFilter[0] = layerProperties[level].filterOperator;
      currentFilter[2] = parseInt(layerProperties[level].filterComparable);
    }
  }
  return inputJSON;
}

function cloneJson (json) {
  return JSON.parse(JSON.stringify(json));
}

const layerProperties = {
  '-3': {
    layerName: 'tunneli2',
    filterOperator: '<',
    filterComparable: '-2'
  },
  '-2': {
    layerName: 'tunneli1',
    filterOperator: '==',
    filterComparable: '-2'
  },
  '-1': {
    layerName: 'alikulku',
    filterOperator: '==',
    filterComparable: '-1'
  },
  0: {
    layerName: 'pinta',
    filterOperator: '==',
    filterComparable: '0'
  },
  1: {
    layerName: 'ylikulku',
    filterOperator: '==',
    filterComparable: '1'
  },
  2: {
    layerName: 'silta1',
    filterOperator: '==',
    filterComparable: '2'
  },
  3: {
    layerName: 'silta2',
    filterOperator: '>',
    filterComparable: '2'
  }
};

main(process.argv[2], process.argv[3]);
