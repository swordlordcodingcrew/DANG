# Tiled Exporter
SwordLord DANG! makes use of the Tiled Editor to design levels. These levels can then be exported into a C++ file to be used in your game.

## How to install
Get the blit32_exporter.js script from the tools folder, save it on your computer (just git clone this repo). And then symlink this script into the configuration folder so that the Tiled Editor can find the script.

``` shell
cd ~/.config/tiled/extensions
ln -s /path/to/the/blit32_exporter.js/on/your_system blit32_exporter.js
```

Restart the Tiled Editor. You can see in the Tiled Console if the script was loaded successfully.


## How to use
Press *Ctrl+P* in the Tiled editor on a Tiled Map to export the current map as a C++ file with the same name as the map.

Or just select *File -> Export32Blit* from the menu. 

You will get a new file with the name of the map and a suffix of .hpp. Add this file to your project.

Make sure that you convert all the spritesheets you used with the Sprite Builder script which you can find in the 32blit tools folder.

## Troubleshoot
Any error will be printed to the Tiled Editor console.
