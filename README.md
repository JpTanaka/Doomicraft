# ⛏️ Doomicraft ⛏️

A mix between Minecraft and Doom! Build your world on Creative or fight creepers in Survival, hoping to last another level...

Made on C++ with the CGP library (built on top of OpenGL) as an learning experiment with graphics programing.

The game is free to play and it uses free assets. 

Originally a project for the INF443 course at Polytechnique.

![Doomicraft creeper](https://user-images.githubusercontent.com/82896115/234225080-24732dd7-2ef3-48d1-82a9-29a3acb35fd0.jpeg)



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

Some images from the gameplay.

![WhatsApp Image 2023-04-25 at 10 33 54](https://user-images.githubusercontent.com/82896115/234225492-ad5f7d6c-9b16-4467-919e-009a5b61931d.jpeg)


![WhatsApp Image 2023-04-25 at 10 43 45](https://user-images.githubusercontent.com/82896115/234225661-6a562558-e966-4095-8b84-c205d9a2fb51.jpeg)
![WhatsApp Image 2023-04-25 at 10 42 19](https://user-images.githubusercontent.com/82896115/234225663-867605e9-211b-4e8d-bba2-a5a9a59a731f.jpeg)
![WhatsApp Image 2023-04-25 at 10 36 03](https://user-images.githubusercontent.com/82896115/234225669-44f7b516-7027-4104-ae04-1ebb751309ab.jpeg)
![WhatsApp Image 2023-04-25 at 10 34 57](https://user-images.githubusercontent.com/82896115/234225680-a87c3a61-2bcc-4310-b27a-91d5f8e1ad49.jpeg)
![Game over](https://user-images.githubusercontent.com/82896115/234225659-4baa08cf-dbe3-4410-8f23-cfba461108af.jpeg)


