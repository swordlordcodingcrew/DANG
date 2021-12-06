# SwordLord DANG!
SwordLord DANG! (Dainty Animations 'N Gears) is an Open Source Game Framework for the [32blit](https://32blit.com/) handheld open game console.

### Games using DANG!

- [Pigs n Kings](https://github.com/swordlordcodingcrew/pigsnkings/)
- Dark Iridium 5

### Why DANG!
The 32blit console is an interesting handheld. Pimoroni did a great job in creating a promising piece of hardware. But having a new platform unfortunately also means that tools and utilities that you are used to and that you became dependent upon are missing.

So we from SwordLord decided to work on an easy-to-use game framework for the 32blit. For us and our own games, but also for everybody else.

### DANG! is not an Engine
While it would be cool to build a game engine for the 32blit, this is not what we try to achieve. We don't have the intention to re-invent Unity or Godot.

We focus on gluing a few existing (and new) tools and pieces together. Think of DANG! as a set of tools in a toolbox where you (hand-) pick the tools you need for a specific job. With DANG! you can focus on what makes your game unique and don't have to invest too much time into the standard things like animations or collision detection.

DANG! is an opinionated framework. Which means while you are free to use only parts and work around others, you will only experience the full power of DANG! when you follow a few rules.

## Features
DANG! is somewhat work in progress. But already quite capable to reduce the time you need to invest to get a game off the ground.

What to expect from and you will get when using DANG!

- Direct export of your levels from [Tiled Editor](https://www.mapeditor.org/) based maps into C++ source code, ready to be used from your own code. No need to convert further, direct access to your data. Best of all, since those level definitions are defined as const struct, even huge levels won't really crash your 32blit handheld.
- Direct export of your Graphics from [Aseprite](https://www.aseprite.org/) into c++ source code. No need for a conversion step neither during compilation, nor when run on the 32blit handheld. Even huge collections of graphics will work.
- If you define animations within the [Tiled Editor](https://www.mapeditor.org/), those will automatically be played in your game. No need to write code for that.
- DANG! contains a powerful **Tween Engine** and some pre-defined tweens which help you in animating and moving sprites.
- Helper functions to play sound fx and a **MOD engine** which plays your favorite module files.
- **Collision detection** which is small and focused on arcade games.
- Support for automatically handling the viewport. Just bind it to a sprite and have the camera follow that sprite, as example.
- A powerful **Behaviour Tree** implementation where you can define the NPCs behaviour in a human-readable, descriptive format. Should your baddies react to the players behaviour? DANG! got you covered.
- Define rumble patterns for the game controller and the 32blit device, store and play them as **Rumble Tracks** exactly as you would play a song.

## Documentation
Have a look at our [Wiki pages](https://github.com/swordlordcodingcrew/DANG/wiki). 

To get details on the code, you can generate the code documentation with the help of doxygen and graphviz.

```
sudo apt-get install doxygen
sudo apt-get install graphviz
```

and then run doxygen in the root of the DANG! project folder (where you can see the Doxyfile).

```
doxygen
```

The documentation will be generated in the /docs/ folder. Check the index file in the html folder.

## Licence
SwordLord DANG! and all its tools are released under the AGPLv3 License. See LICENSE file for details. Drop us a note if you need another license.
