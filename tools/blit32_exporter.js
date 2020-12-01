// (c) 2019-20 by SwordLord - the coding crew
// this file will export any tilemap to its 32blit cpp representation
// This file is part of the SwordLord DANG! Game Framework.
 
// format definition
var customMapFormat = {
    name: "32blit map format",
    extension: "32blit",
    write: export32Blit,
}

// register new format with tiled
tiled.registerMapFormat("32blit", customMapFormat);

// define new action (can be called by user directly)
var action = tiled.registerAction("Export32Blit", function(action) {

    asset = tiled.activeAsset;
    if(asset.isTileMap){
        tiled.log("export called, exporting to 32blit.");
        export32Blit(asset, asset.fileName + ".hpp")
        tiled.log("File created at: " + asset.fileName + ".hpp");
    }
    else {
        tiled.log("export called, but wrong asset type active.");
    }
});

action.text = "Export32Blit";
action.shortcut = "Ctrl+P";

// register action in file menu (right above the export)
tiled.extendMenu("File", [
    { action: "Export32Blit", before: "Export" },
    { separator: true }
]);

// the export function
function export32Blit(map, fileName) {

    var file = new TextFile(fileName, TextFile.WriteOnly);

    var mTS = new Map();

    segments = fileName.split("/");
    lastSegment = segments[segments.length - 1];

    segments = lastSegment.split(".");
    functionName = segments[0];

    file.writeLine("// This file is auto generated by the SwordLord DANG Tiled Exporter.");
    file.writeLine("// It will get overwritten once you re-export your tile map!");
    file.writeLine("// (c) 2019-20 by SwordLord - the coding crew")
    file.writeLine("");

//    file.writeLine("#include \"libs/dang/src/Imagesheet.h\"")
    file.writeLine("");

    file.writeLine("dang::tmx_level init_" + functionName + "() {");
    file.writeLine("");

    file.writeLine("    dang::tmx_level lvl = {};");

    file.writeLine("");

    file.writeLine("// World ------------------------------------------------");
    file.writeLine("");
    file.writeLine("    lvl.w.width = " + map.width + ";");
    file.writeLine("    lvl.w.height = " + map.height + ";");
    file.writeLine("    lvl.w.tileWidth = " + map.tileWidth + ";");
    file.writeLine("    lvl.w.tileHeight = " + map.tileHeight + ";");

    // not sure if only an accident or we really need to move from top to bottom...
    //tilesets = map.usedTilesets();
    file.writeLine("");
    file.writeLine("// Tilesets ------------------------------------------------");
    file.writeLine("");

    tilesets = map.tilesets;
    for (var i = 0; i < tilesets.length; ++i) {
        ts = tilesets[i];

        // assuming no margin and no spacing
        var cols = Math.floor(ts.imageWidth / ts.tileWidth);
        var rows = Math.floor(ts.imageHeight / ts.tileHeight);

        mTS.set(ts.name, i);
        file.writeLine("    // Tileset: " + ts.name + " has ID: " + i);
        file.writeLine("    lvl.tilesets[" + i + "] = {\"" + ts.name + "\"," + ts.tileCount + "," + ts.tileWidth + "," + ts.tileHeight +
            "," + ts.imageWidth + "," + ts.imageHeight + "," + cols + "," + rows + "};");
    }

    file.writeLine("");
    file.writeLine("// Layers ------------------------------------------------");
    //file.writeLine("const uint8_t LAYER_COUNT=" + map.layerCount + ";");
    //file.writeLine("layer* layers[LAYER_COUNT];");
    file.writeLine("");

    for (var i = 0; i < map.layerCount; ++i) {
        var layer = map.layerAt(i);
        if (layer.isTileLayer) {
            file.writeLine("    // layer tilelayer: " + layer.name);

            var buf = "\n";
            //file.writeLine("tile t_" + layer.name + "[" + layer.height + "][" + layer.width + "] = {");
// flip possibilities in 32blit
//       NONE = 0b000,          0
//       HORIZONTAL = 0b001,    1
//       VERTICAL = 0b010,      2
//       XYSWAP = 0b100,        4
//       R90 = 0b101,           5
//       R180 = 0b011,          3
//       R270 = 0b110           6

            for (y = 0; y < layer.height; ++y) {

                for (x = 0; x < layer.width; ++x) {

                    tile = layer.tileAt(x, y);
                    cell = layer.cellAt(x, y);

                    var transform = 0;
                    if (cell.flippedHorizontally) {
                        transform = 1;
                    }
                    else if (cell.flippedVertically) {
                        tranform = 2;
                    }
                    else if (cell.flippedAntiDiagonally) {
                        transform = 4;
                    }
                    buf += "        {";
                    buf += tile.id + ",";
                    buf += mTS.get(tile.tileset.name) + ",";
                    buf += transform;
//                    buf += (cell.flippedHorizontally | 0) + ",";
//                    buf += (cell.flippedVertically | 0) + ",";
//                    buf += (cell.flippedAntiDiagonally | 0);
                    buf += "}";

                    if(x < layer.width - 1)
                    {
                        buf += ",";
                        buf += ("\n");
                    }
                }
                if(y < layer.height - 1){
                    //file.write(",");
                    buf += ",";
                }
                buf += ("\n");
            }
            //file.writeLine("};");
            //file.writeLine("layer l_" + layer.name + " = {\"" + layer.name + "\"," + layer.opacity + "," + layer.visible + ",0," + "t_" + layer.name + "};");

            file.writeLine("    dang::tmx_tile t" + layer.name + "[] = {" + buf + "    };");

            var size = layer.width * layer.height;

            file.writeLine("    dang::tmx_tilelayer tl" + layer.name + " = {\"" + layer.name + "\"," + size + ",t" + layer.name + "," + layer.width + "," + layer.height + "};");
            file.writeLine("    lvl.layers.push_back(std::make_shared<dang::tmx_tilelayer>(tl" + layer.name + "));");

        }
        else if (layer.isObjectLayer) {

            file.writeLine("");
            file.writeLine("    // Objects for layer: " + layer.name);

            var buf = "\n";

            objects = layer.objects

//	    file.writeLine("	// number of objects: " + objects.length)

            for (var j = 0; j < objects.length; ++j)
            {
                o = objects[j]
//		        file.writeLine(j + ": " + o.type + "; " + o.tile)
                sName = o.name;
                if(sName == ""){
                    sName = o.id
                }

                //file.writeLine("spriteobject so_" + sName + " = {" + o.id + ",\"" + o.type + "\"," + o.x + "," + o.y + "," + o.width + "," + o.height + "," + o.visible + "," + mTS.get(o.tile.tileset.name) + "," + o.tile.id + "};");
                var tileset_ref = "0"
                var tile_id = "0"
                if (o.tile != null)
                {
                    tileset_ref = mTS.get(o.tile.tileset.name)
                    tile_id = o.tile.id
                }
                buf += "        {" + o.id + ",\"" + sName + "\",\"" + o.type + "\"," + o.x + "," + o.y + "," + o.width + "," + o.height + "," + o.visible + "," + tileset_ref + "," + tile_id + "}";
                if(j < objects.length - 1){
                    //file.write(",");
                    buf += ",";
                }
                buf += "\n";

            }
            file.writeLine("    dang::tmx_spriteobject so" + layer.name + "[] = {" + buf + "    };");

            file.writeLine("    dang::tmx_objectlayer l" + layer.name + " = {\"" + layer.name + "\"," + objects.length + ",so" + layer.name + "};");
            file.writeLine("    lvl.layers.push_back(std::make_shared<dang::tmx_objectlayer>(l" + layer.name + "));");

        //file.writeLine("layer l_" + layer.name + " = {\"" + layer.name + "\"," + layer.opacity + "," + layer.visible + ",1," + "&so_" + sName + "};");
        }
    }

    /*std::string name;
    float opacity;
    bool visible;
    // TODO: to be considered
    //offset : point 	Offset in pixels that is applied when this layer is rendered.
    //map : TileMap 	Map that this layer is part of (or null in case of a standalone layer).
    void* pLayer;*/

    file.writeLine("");

    file.writeLine("    // make sure to generate every file with the 32blit-tool and link them.");
    mTS.forEach(function(value, key)
    {
        file.writeLine("    // " + key + ".h");
    })

    file.writeLine("");

    // TODO     lvl.imagesheets["players"] = Imagesheet::load(players, nullptr, 8, 3); where 8 = cols and 3 = rows
//    mTS.forEach(function(value, key)
//    {
//        file.writeLine("    lvl.imagesheets[\"" + key + "\"] = dang::Imagesheet::load(" + key + ");");
//    })

    mTS.forEach(function(value, key)
    {
        file.writeLine("    lvl.images[\"" + key + "\"] = " + key + ";");

    })

//    for (var i = 0; i < tilesets.length; ++i)
//    {
//        ts = tilesets[i];
//
//       mTS.set(ts.name, i);
//        file.writeLine("    // Tileset: " + ts.name + " has ID: " + i);
//        //file.writeLine("// #include \"tileset_name.hpp\"");
//        //file.writeLine("tileset ts_" + ts.name + " = {\"" + ts.name + "\"," + ts.tileCount + "," + ts.tileWidth + "," + ts.tileHeight + "};");
//        file.writeLine("    lvl.tilesets[" + i + "] = {\"" + ts.name + "\"," + ts.tileCount + "," + ts.tileWidth + "," + ts.tileHeight + "};");
//    }

    //file.writeLine("");
//    mTS.forEach(function(value, key) {
        //file.writeLine("    tilesets[" + value + "] = &ts_" + key + ";");
//    })

    //file.writeLine("");
    for (var i = 0; i < map.layerCount; ++i) {
        var layer = map.layerAt(i);
        //file.writeLine("    layers[" + i + "] = &l_" + layer.name + ";");
    }

    file.writeLine("");
    file.writeLine("    return lvl;");
    file.writeLine("}");

    file.commit();
    file.close();
}
