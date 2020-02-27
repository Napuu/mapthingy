module.exports = {
  "vesialue": [{
    "layer": "vesialue",
    "minimum-zoom": "7",
    "maximum-zoom": "14",
    "simplify-only-low-zooms": true,
    "simplification": 5,
    "detect-shared-borders": true,
    "coalesce-densest-as-needed": true
  }],
  "maankaytto": [{ // quite big tiles right now, maybe use corine at higher zoom levels (?)
    "layer": "maankaytto",
    "minimum-zoom": "7",
    "maximum-zoom": "14",
    "simplify-only-low-zooms": true,
    "no-tile-size-limit": true,
    "simplification": 5,
    "detect-shared-borders": true,
    "coalesce-densest-as-needed": true
  }],
  "rakennus": [{
    "layer": "rakennus",
    "minimum-zoom": "12",
    "maximum-zoom": "14",
    "detect-shared-borders": true,
  }],
  "reunaviiva": [{
    "layer": "reunaviiva",
    "minimum-zoom": "8",
    "maximum-zoom": "12",
    "no-tile-size-limit": true,
    "simplification": 5,
  }],
  "tieviiva": [{
    "layer": "isotie",
    "minimum-zoom": "6",
    "maximum-zoom": "10",
    "feature-filter": "{\"*\": [ \"any\", [\"<\", \"tienumero\", 10000] ] }"
  },{
    "layer": "polku",
    "minimum-zoom": "13",
    "maximum-zoom": "14",
    "feature-filter": "{\"*\": [ \"any\", [\">\", \"kohdeluokka\", 12300] ] }"
  }],
  "rautatie": [{
    "layer": "rautatie",
    "minimum-zoom": "6",
    "maximum-zoom": "12"
  }],
  "paikannimi": [{
    "layer": "paikannimi",
    "no-tile-size-limit": true,
    "no-feature-limit": true,
    "minimum-zoom": "6",
    "maximum-zoom": "12",
    "drop-rate": "0"
  },{
    "layer": "kunnannimi",
    "no-feature-limit": true,
    "minimum-zoom": "4",
    "maximum-zoom": "12",
    "drop-rate": "0",
    "feature-filter": "{\"*\": [ \"any\", [\"in\", \"kohdeluokka\", 48111, 48112] ] }"
  }]
}
