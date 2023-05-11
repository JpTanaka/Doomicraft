<p align="center">
  <img src = "https://github.com/JpTanaka/Doomicraft/assets/77277361/cd095d24-18f5-407e-a63f-c64d45b7daff" alt="Doomicraft"/>
</p>

A mix between Minecraft and Doom! Build your world on Creative or fight creepers in Survival, hoping to last another level...

Made on C++ with the CGP library (built on top of OpenGL) as an learning experiment with graphics programing.

The game is free to play and it uses free assets. 

Originally a project for the INF443 course at Polytechnique.

Authors: Guile Vieira (guilevieiram), João Tanaka (jptanaka) and Rafael Nagai

![Screenshot from 2023-05-11 14-15-49](https://github.com/JpTanaka/Doomicraft/assets/77277361/ab4bd509-fe5d-4d7f-9649-c4f453abb4a7)


## 📥 Downloading
To download the latest version of the game refer to the releases tab of the GitHub page.
After downloading the package (.tar for linux and .zip for windows), go into the folder Doomicraft, then into build and run (double click) on Doomicraft!
The path is going to look like: `Doomicraft/build/Doomicraft[.exe]`;

The game should start running on your machine after that!

You are encouraged to create a shortcut on your Desktop workspace if you want to access without going into the folder structure.

## 💽 Compiling 
If you want to compile the game by yourself on your linux machine, go through with the following steps:

0. Make sure you have CMake on version at least 3.8 and git installed on your machine.

1. clone the project:
```bash
git clone https://github.com/JpTanaka/Doomicraft
cd Doomicraft
```

2. Initialize the submodules
```bash
git submodule init
git submodule update
```

3. Create the build
```bash
cmake -B build
cd build
```

4. Compile everything
```bash
make
```
5. Run the game!
```bash
./Doomicraft
```
## Galery

Videos of the gameplay showing the game features:
- Creative: https://youtu.be/k5S0gGmGR48
- Survival: https://youtu.be/ag2cmsAGo3w

Some images from the gameplay.

![Screenshot from 2023-05-11 14-17-59](https://github.com/JpTanaka/Doomicraft/assets/77277361/d011f25b-d809-4072-9ce3-e6980ecbaf67)
![Screenshot from 2023-05-11 14-17-22](https://github.com/JpTanaka/Doomicraft/assets/77277361/92623790-ffe8-4ff7-9a78-1fcec652c28a)
![Screenshot from 2023-05-11 14-15-59](https://github.com/JpTanaka/Doomicraft/assets/77277361/c4702868-4642-40cf-a299-f0b64388229c)
![Screenshot from 2023-05-11 14-15-08](https://github.com/JpTanaka/Doomicraft/assets/77277361/1e83807e-47f4-4f7f-8c61-33b6771f8a62)



## Implementation details

This game was completely implemented in C++ using the CGP library (linked as a submodule) for graphics rendering. OOP was the main paradigm chosen for the project even though some new C++ features (like lambda functions) were used from time to time.

Many algorithms and implementations were implemented in the construction of the program. We will in this section highlight the main ones.


--------------------------------------
### Rendering
To achieve high performance on the game (at least 30FPS), some modifications on the block rendering were made. Initially, each block is a collection of 6 square faces, each one with a texture.

1. The meshes for each type of block are created and loaded on the GPU only once in the initialization of the program. The drawing of different blocks is made using translations. The same works for the flowers, they are loaded in the GPU only once.
2. The contact face when two blocks are touching are not rendered, which decreases significantly the render time for the scene.
3. A max depth to render blocks is defined to prevent blocks far away to be rendered. This, together with the Fog (implemented on the Shader) gives a smooth transition between the terrain and the background. Also the number of render calls decreses significantly with this optimization. The fog depth can be changed by the player on the menu on runtime!
4. The faces whose normal face away from the direction the player is looking are not shown. This is made by approximation using a threshold on the dot product of the vectors since the calculations using the field of view would be too expensive.

--------------------------------------
### Collision
Two collision algorithms were implemented, one for the main player and one for the mobs/creepers. Both of them are based on AABB (Axis Aligned Bounding Boxes) collision that fits well the game theme.

1. **The Mob Collision: AABB vs AABB**. 
In this algorithm we check, at each point in time if the bounding box from the creeper (two blocks on top of eachother) is going to collide with the blocks around it. If there is going to be a collision, we filter the move direction to prevent it. By looking only at a achievable subset of blocks, this approach is pretty fast and adaptable to implement the mob AI. Nonetheless, it leads to undeterministic behaviour and tunneling, this is why we implemented another algorithm for the main player.

2. **The Player Collision: Swept AABB vs AABB**. 
For this algorithm the main idea is to simulate the player movement in time and stop at the earliest time of collision with another block. Although it is more expensive, it prevents tunnelling and allows for movement response. For a better playability, we implemented a sliding response to collision with the terrain and we modeled the player using a smaller parallepiped (to allow 1 by 1 tunnels). More information on this algorithm can be found on the page https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/



--------------------------------------
### Terrain Generation
To gerenate infinite terrain for the game, a few approaches were taken:
1. We used Perlin Noise to determine the height of the terrain.
2. Trees and flowers were placed by simulating a uniform random variable at each block. A check to see if another block was already in place was put in place.
3. A Chunk model was implemented to increase performance and allow for infinite worlds. A chunk is a 16 by 16 (on x and y) of the terrain. The terrain rendering and collisions with blocks are checked only in the surrounding chunks of the player, wich allows for a constant time check independently of the size of the world. New chunks are generated at need when the player moves. Breaking and placing blocks alter the chunk permanently so that changes persist for the rest of the game.


--------------------------------------
### Modeling and Texturing
Most of the modeling was done using CGP primitive shapes (squares, cubes, ...) and some were made using blender. 

The gun was downloaded as a free-to-use asset and modified to our needs using blender. 
The blocks and flowers textures were downloaded from free-to-use Minecraft texture packs and adapted to our system.
The creeper art was made by hand on Gimp and used as a texture for the mobs.


--------------------------------------
### SFX
To use sound effects in the game a header only library (miniaudio) was used. This API allows the C++ program to play sounds on the main audio driver of the computer, but this execution blocks the execution of the main thread. To surpass this and play different sounds at different times, we implemented a threaded system that spawns one thread for each SFX that stays alive until the end of the game. This thread accesses a set of variables called listeners that dictate if the given sound should be played or not. This allows for other objects (like the player or terrain) to activate sounds. The assets were taken from free-to-use libraries and adapted to our needs (volume, pitch, duration, ...) using the Audacity software.


--------------------------------------
### Gameplay
Two main gameplay modes were implemented: Survival and Creative. On the survival mode the objective is to kill as many mobs as possible as the level increases. On the creative you have more time and resources to explore and build!

1. A mob group controller that keeps track of deaths, lifes and spawns. At every ~20 seconds (or when a mob dies) another creeper spawns.
2. Each mob is given an AI that follows the player. In encountering terrain, it jumps and continues moving towards the player.
3. The player was given 5 lifes (looses one at each touch of a creeper), a respawn timed system, the hability to place and destroy different blocks, ...
4. A UI with the player status (life, kills and level) and another with the blocks are given to allow for smoother gameplay. This was implemented using the ImGUI library. 
5. A main menu was implemented to allow for game pausing, exiting and other configurations. These configurations can be accessed in the debug mode and include a creative mode, change fog depth, see wireframes and other functionalities.
6. A crosshair with a hitmark sensor was given to the player to allow aiming. To do this, the creation of timed GUI elements were needed. This system was then used to display other messages on the screen for a fixed amount of time.
