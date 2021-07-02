// (c) 2019-2021 by SwordLord - the coding crew
// Add this script to your installation of the Tiled Map Editor. With this script you can export your tilemaps to their cpp representation.
//
// This file is part of the SwordLord DANG! Game Framework. DANG! is made for the 32blit Gaming Hardware.

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
action.shortcut = "Ctrl+D";

// register action in file menu (right above the export)
tiled.extendMenu("File", [
    { action: "Export32Blit", before: "Export" },
    { separator: true }
]);

// the export function
function export32Blit(map, fileName) {

    var bHasWaypoints = false;

    var file = new TextFile(fileName, TextFile.WriteOnly);

    var mTS = new Map();

    segments = fileName.split("/");
    lastSegment = segments[segments.length - 1];

    segments = lastSegment.split(".");
    functionName = segments[0];

// header =============================================================================================================

    file.writeLine("// This file is auto generated by the SwordLord DANG Tiled Exporter.");
    file.writeLine("// It will get overwritten once you re-export your tile map!");
    file.writeLine("// (c) 2019-2021 by SwordLord - the coding crew");
    file.writeLine("");

// world ==============================================================================================================

    file.writeLine("// World ------------------------------------------------");
    file.writeLine("");
    file.writeLine("static const dang::tmx_world " + functionName + "_world = {");
    file.writeLine("    .width = " + map.width + ",");
    file.writeLine("    .height = " + map.height + ",");
    file.writeLine("    .tileWidth = " + map.tileWidth + ",");
    file.writeLine("    .tileHeight = " + map.tileHeight);
    file.writeLine("};");
    file.writeLine("");

// Tilesets ===========================================================================================================

    // not sure if only an accident or we really need to move from top to bottom...
    //tilesets = map.usedTilesets();
    file.writeLine("// Tilesets ------------------------------------------------");
    file.writeLine("");
    file.writeLine("static const dang::tmx_tileset " + functionName + "_tilesets[] = {");

    tilesets = map.tilesets;
    for (var i = 0; i < tilesets.length; ++i)
    {
        ts = tilesets[i];

        mTS.set(ts.name, i);

        // assuming no margin and no spacing
        var cols = Math.floor(ts.imageWidth / ts.tileWidth);
        var rows = Math.floor(ts.imageHeight / ts.tileHeight);

        var comma = "";
        if (i < tilesets.length)
        {
            comma = ",";
        }
        file.writeLine("    {\"" + ts.name + "\", &" + ts.name + ", " + ts.tileCount + "," + ts.tileWidth + "," + ts.tileHeight +
            "," + ts.imageWidth + "," + ts.imageHeight + "," + cols + "," + rows + "}" + comma);
    }
    file.writeLine("};");
    file.writeLine("");
    file.writeLine("static const size_t " + functionName + "_tilesets_len = " + tilesets.length + ";");
    file.writeLine("");

// Animations =========================================================================================================

    file.writeLine("// Animations ------------------------------------------------");
    file.writeLine("");


    file.writeLine("static const dang::tmx_tileanimation " + functionName + "_tileanimations[] = {");

    var animationId = 0;
    for (var i = 0; i < tilesets.length; ++i)
    {
        ts = tilesets[i];

        for (var j = 0; j < ts.tiles.length; ++j)
        {
            t = ts.tiles[j];

            // only export them if they have a name
            if (t.animated && t.type.length > 0)
            {
                var buf = "";
                for (var k = 0; k < t.frames.length; ++k)
                {
                    f = t.frames[k];

                    if (buf.length > 0)
                    {
                        buf += ",";
                    }
                    buf += "{" + f.tileId + ", " + f.duration + "}";
                }

                file.writeLine("    {\"" + ts.name + "\"," + t.id + ",\"" + t.type + "\",{" + buf + "}},");
                animationId++;
            }
        }
    }

    file.writeLine("};");
    file.writeLine("");
    file.writeLine("static const size_t " + functionName + "_tileanimations_len = " + animationId + ";");
    file.writeLine("");

// Layers =============================================================================================================
// flip possibilities in 32blit
//       NONE = 0b000,          0
//       HORIZONTAL = 0b001,    1
//       VERTICAL = 0b010,      2
//       XYSWAP = 0b100,        4
//       R90 = 0b101,           5
//       R180 = 0b011,          3
//       R270 = 0b110           6

    file.writeLine("// Layers ------------------------------------------------");
    file.writeLine("");

    for (var i = 0; i < map.layerCount; ++i)
    {
        var layer = map.layerAt(i);
        if (layer.isTileLayer)
        {
            file.writeLine("// layer tilelayer: " + layer.name);

            var buf = "\n";
            //file.writeLine("tile t_" + layer.name + "[" + layer.height + "][" + layer.width + "] = {");

            for (y = 0; y < layer.height; ++y)
            {
                for (x = 0; x < layer.width; ++x)
                {
                    tile = layer.tileAt(x, y);
                    cell = layer.cellAt(x, y);

                    var transform = 0;
                    if (cell.flippedHorizontally)
                    {
                        transform = 1;
                    }
                    else if (cell.flippedVertically)
                    {
                        transform = 2;
                    }
                    else if (cell.flippedAntiDiagonally)
                    {
                        transform = 4;
                    }

                    if (x == 0)
                    {
                        buf += "   ";
                    }

                    buf += " {" + tile.id + ", " + transform + "}";

                    if (x < layer.width - 1)
                    {
                        buf += ",";
                    }
                }

                if (y < layer.height - 1)
                {
                    buf += ",";
                }

                buf += ("\n");
            }

            file.writeLine("static const dang::tmx_tile " + functionName + "_" + layer.name + "_tiles[] = {");
            file.writeLine(buf);
            file.writeLine("};");
            file.writeLine("");
            file.writeLine("static const size_t " + functionName + "_" + layer.name + "_tiles_len = " + layer.width * layer.height + ";");
            file.writeLine("");
            file.writeLine("static const dang::tmx_layer " + functionName + "_" + layer.name + " = {");
            file.writeLine("    .name = \"" + layer.name + "\",");
            file.writeLine("    .type = dang::tmx_layerType::ltTile,");
            file.writeLine("    .opacity = " + layer.opacity + ",");
            file.writeLine("    .visible = " + layer.visible + ",");
            file.writeLine("    .z_order = " + i + ",");
            file.writeLine("    .tl_width = " + layer.width + ",");
            file.writeLine("    .tl_height = " + layer.height + ",");
            file.writeLine("    .tl_tileset = \"" + layer.tileAt(0,0).tileset.name + "\",");
            file.writeLine("    .tl_tiles = " + functionName + "_" + layer.name + "_tiles,");
            file.writeLine("    .tl_tiles_len = " + functionName + "_" + layer.name + "_tiles_len,");
            file.writeLine("    .spriteobjects = nullptr,");
            file.writeLine("    .spriteobejcts_len = 0");
            file.writeLine("};");
            file.writeLine("");

        }
        else if (layer.isObjectLayer)
        {
            // check if this layer has points. If this is the case the layer is assumed to be the path-layer
            if (layer.objects.length > 0 && layer.objects[0].shape == MapObject.Point)
            {
                bHasWaypoints = true; // so that we only link waypoints at the end if there really are...
                file.writeLine("// layer with points - path layer");

                objects = layer.objects

                var wap_len = 0;
                var conn_len = 0;

                var buf_pts = "static const dang::tmx_waypoint " + functionName + "_waypoints[] = {\n";
                var buf_conn = "static const dang::tmx_waypoint_connection " + functionName + "_connections[] = {\n";

                for (var j = 0; j < objects.length; ++j)
                {
                    var o = objects[j];
                    // ugly hack since iteration over custom props is not possible
                    for (var k = 0; k < 10; ++k)
                    {
                        var val = o.property("walk_" + k);
                        if (val != undefined)
                        {
                            buf_conn += "    {" + o.id + ", " + val.id + ", 0x1},\n";
                            conn_len++;
                        }
                    }

                    for (var k = 0; k < 10; ++k)
                    {
                        var val = o.property("jump_" + k);
                        if (val != undefined)
                        {
                            buf_conn += "    {" + o.id + ", " + val.id + ", 0x2},\n";
                            conn_len++;
                        }
                    }

                    buf_pts += "    {" + o.id + ", " + o.x + "," + o.y + ", " + o.type + "},\n";
                    wap_len++;
                }

                buf_pts += "};";
                buf_conn += "};";
                file.writeLine(buf_pts);
                file.writeLine("");
                file.writeLine("static const size_t " + functionName + "_waypoints_len = " + wap_len + ";")
                file.writeLine("");

                file.writeLine(buf_conn);
                file.writeLine("");
                file.writeLine("static const size_t " + functionName + "_connections_len = " + conn_len + ";")
                file.writeLine("");
            }
            else
            {
                file.writeLine("// Objects for layer: " + layer.name);

                var buf = "\n";

                objects = layer.objects

                for (var j = 0; j < objects.length; ++j)
                {
                    o = objects[j]
                    sName = o.name;
                    if (sName == "")
                    {
                        sName = o.id;
                    }

                    var tileset_ref = "";
                    var tile_id = "0";
                    if (o.tile != null)
                    {
                        tileset_ref = o.tile.tileset.name;
                        tile_id = o.tile.id;
                    }

                    sBT = "";
                    if (o.property("bt") != undefined)
                    {
                        sBT = o.property("bt");
                    }

                    var sTransform = "0b0"; // this is prefix and bit 3
                    sTransform += o.FlippedAntiDiagonally ? "1" : "0"; // not sure that one works...
                    sTransform += o.tileFlippedVertically ? "1" : "0";
                    sTransform += o.tileFlippedHorizontally ? "1" : "0";

                    buf += "    {" + o.id + ",\"" + sName + "\",\"" + o.type + "\"," + o.x + "," + o.y + "," + o.width + "," + o.height + "," + o.visible + ",\"" + tileset_ref + "\"," + tile_id + ",\"" + sBT + "\"," + sTransform + "}";

                    // add separator except at the end
                    if (j < objects.length - 1)
                    {
                        buf += ",";
                    }
                    buf += "\n";

                }

                file.writeLine("static const dang::tmx_spriteobject " + functionName + "_" + layer.name + "_objects[] = {");
                file.writeLine(buf);
                file.writeLine("};");
                file.writeLine("");
                file.writeLine("static const size_t " + functionName + "_" + layer.name + "_objects_len = " + objects.length + ";");
                file.writeLine("");
                file.writeLine("static const dang::tmx_layer " + functionName + "_" + layer.name + " = {");
                file.writeLine("    .name = \"" + layer.name + "\",");
                file.writeLine("    .type = dang::tmx_layerType::ltObjects,");
                file.writeLine("    .opacity = " + layer.opacity + ",");
                file.writeLine("    .visible = " + layer.visible + ",");
                file.writeLine("    .z_order = " + i + ",");
                file.writeLine("    .tl_width = 0,");
                file.writeLine("    .tl_height = 0,");
                file.writeLine("    .tl_tileset = \"\",");
                file.writeLine("    .tl_tiles = nullptr,");
                file.writeLine("    .tl_tiles_len = 0,");
                file.writeLine("    .spriteobjects = " + functionName + "_" + layer.name + "_objects,");
                file.writeLine("    .spriteobejcts_len = " + functionName + "_" + layer.name + "_objects_len");
                file.writeLine("};");
                file.writeLine("");
            }   // end of sprite object layer

        }   // end of object layer

    } // end of layer

    file.writeLine("const static dang::tmx_layer " + functionName + "_layers[] = {");

    for (var i = 0; i < map.layerCount; ++i)
    {
        var layer = map.layerAt(i);
        if (layer.isObjectLayer)
        {
            if (layer.objects.length > 0 && layer.objects[0].shape != MapObject.Point)
            {
                file.writeLine("    " + functionName + "_" + layer.name + ",");
            }
        }
        else
        {
            file.writeLine("    " + functionName + "_" + layer.name + ",");
        }
    }
    file.writeLine("};");
    file.writeLine("");
    file.writeLine("const static size_t " + functionName + "_layers_len = " + map.layerCount + ";");
    file.writeLine("");

    // TODO: to be considered
    //offset : point 	Offset in pixels that is applied when this layer is rendered.
    //map : TileMap 	Map that this layer is part of (or null in case of a standalone layer).

// Level ==============================================================================================================

    file.writeLine("// Level ---------------------------------------------------");
    file.writeLine("");
    file.writeLine("static const dang::tmx_level " + functionName + "_level {");
    file.writeLine("    .w = &" + functionName + "_world,");
    file.writeLine("    .tilesets = " + functionName + "_tilesets,");
    file.writeLine("    .tilesets_len = " + functionName + "_tilesets_len,");
    file.writeLine("    .tileanimations = " + functionName + "_tileanimations,");
    file.writeLine("    .tileanimations_len = " + functionName + "_tileanimations_len,");
    file.writeLine("    .layers = " + functionName + "_layers,");
    file.writeLine("    .layers_len = " + functionName + "_layers_len,");

    // only have the following lines if
    if (bHasWaypoints) {
        file.writeLine("    .waypoints = " + functionName + "_waypoints,");
        file.writeLine("    .waypoints_len = " + functionName + "_waypoints_len,");
        file.writeLine("    .waypoint_connections = " + functionName + "_connections,");
        file.writeLine("    .waypoint_connections_len = " + functionName + "_connections_len");
    }

    file.writeLine("};");
    file.writeLine("");



// wash hands ==========================================================================================================

    file.writeLine("");
    file.commit();
    // file.close(); -> not needed anymore, commit closes file automatically
}
