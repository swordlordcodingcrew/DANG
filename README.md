# SwordLord DANG!
SwordLord DANG! (Dainty Animations 'N Gears) is an Open Source Game Framework for the [32blit](https://32blit.com/) handheld open game console.

This is work in progress. Please drop us a note if you are interested to help.

### Why DANG!
The 32blit console is an interesting handheld. Pimoroni did a great job in creating a promising piece of hardware. But having a new platform unfortunately also means that tools and utilities that you are used to and that you became dependent upon are missing.

So we from SwordLord decided to work on an easy to use game framework for the 32blit. For us and our own games, but also for everybody else.

### DANG! is not an Engine
While it would be cool to build a game engine for the 32blit, this is not what we try to achieve. We don't have the intention to re-invent Unity or Godot.

We work on gluing a few existing (and new) tools and pieces together. Think of things as a set of tools in a toolbox where you (hand-) pick the tools you need for a specific job.

DANG! is an opinionated framework. Which means while you are free to use only parts, you will only experience the full power of DANG! when you follow a few rules.

## Documentation
You can generate the code documentation with the help of doxygen and graphviz.

```
sudo apt-get install doxygen
sudo apt-get install graphviz
```

and then run doxygen in the root of the DANG project folder (where you can see the Doxyfile).

```
doxygen
```
 
The documentation will be generated in the /docs/ folder. Check the index file in the html folder.

## Architecture
Bear with us while DANG! slowly grows and we start to document and release everything. A quick overview for now:

- Design your levels in [Tiled Editor](https://www.mapeditor.org/).
- DANG! contains a [Tiled Editor](https://www.mapeditor.org/) script which can be used to export your maps. Instead of generating JSON or XML, the DANG! exporter will write C++ source code containing the relevant data.
- If you draw your sprites and graphics with [Aseprite](aseprite.org) (which we think you should), we got you covered. There is an export script to convert your graphics directly into the format used by DANG (and the 32Blit SDK). No need to use the 32blit tool to do so.
- There is a bash script which helps you in exporting your maps and all relevant sprites in one simple step.
- Then use the DANG! framework to write your game. DANG! helps with:
    - in-memory representation of your levels.
    - helper functions to manage sprites and objects.
    - out of the box animations as defined in the tiled map.
    - tween functions to move and animate your sprites.
    - collision detection and (extendable) reactions.
    - support for automated viewport handling.

## Licence
SwordLord DANG! and all its tools are released under the AGPLv3 License. See LICENSE file for details. Drop us a note if you need another license.
