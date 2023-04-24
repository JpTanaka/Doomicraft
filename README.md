# ⛏️ Doomicraft ⛏️

A mix between Minecraft and Doom! Build your world on Creative or fight creepers in Survival, hoping to last another level...

Made on C++ with the CGP library (built on top of OpenGL) as an learning experiment with graphics programing.

The game is free to play and it uses free assets. 

Originally a project for the INF443 course at Polytechnique.

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

