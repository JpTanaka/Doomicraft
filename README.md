# Doomicraft

A mix between Minecraft and Doom! Build your world on Creative or fight creepers in Survival, hoping to last another level...


Made on C++ with the CGP library (built on top of OpenGL) as an learning experiment with graphics programing.

The game is free to play and it uses free assets. Supported on Unix based machines for now (Linux and Mac).

The project can be configured for windows if you have Visual Studio installed (process not covered here)

Originally a project for the INF443 course at Polytechnique.


## Compiling and running
To compile the program, make sure you have CMake on version at least 3.8 and git installed on your machine.
After that, follow this steps:

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

