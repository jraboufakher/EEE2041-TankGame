# EEE2041 — COMPUTER VISION & GRAPHICS
## Tank Game (OpenGL/GLUT)

A small tank game implemented with OpenGL (GLSL) and GLUT, created for **EEE2041 — COMPUTER VISION & GRAPHICS**.

This repository is **self‑contained**:
- Source code (including `common/` utilities)
- Shaders (`shader.vert`, `shader.frag`)
- Runtime assets in `./models/` (OBJ meshes, BMP textures, and level maps)

---

## Features (from code)
- **GLSL pipeline**
  - Vertex shader: `shader.vert` (attributes: `aVertexPosition`, `aVertexTexcoord`; uniforms: `MVMatrix_uniform`, `ProjMatrix_uniform`)
  - Fragment shader: `shader.frag` (uniform: `TextureMap_uniform`)
  - Shaders loaded via `Shader::LoadFromFile("shader.vert","shader.frag")`.
- **Geometry & Textures**
  - OBJ mesh loading/drawing via `common/Mesh.{h,cpp}`
  - BMP texture loading via `common/Texture.{h,cpp}`.
- **Math Utilities**
  - 4×4 matrix operations via `common/Matrix.{h,cpp}` for model–view and projection transforms.
- **Camera**
  - Spherical camera manipulator (`common/SphericalCameraManipulator.{h,cpp}`) for mouse-driven pan (cursor hidden in window).
- **Gameplay (from `main.cpp`)**
  - Tank with chassis, turret, and wheels; turret pans with mouse.
  - Fire a ball from the turret; simple range/interaction checks.
  - Collectible coins; per-level time limit; “Level Completed” / “Out of Time” states.
  - Middle-click context menu: **Pause**, **Continue**, **Exit**.
  - Levels loaded from text files in `./models/` (`Map1.txt`, `Map2.txt`, `Map3.txt`).

---

## Controls
- **W / S** – accelerate forward / backward  
- **A / D** – rotate tank left / right  
- **Space** – set & fire the ball  
- **Mouse move** – rotate turret (pan only; tilt locked)  
- **F** – set window to 1920×1080 and enter fullscreen  
- **O** – set window to 720×720 (windowed)  
- **ESC** – quit  
- **Middle mouse button** – open menu (**Pause / Continue / Exit**)

---

## Project Structure
```
common/
  Matrix.h / Matrix.cpp
  Mesh.h   / Mesh.cpp
  Shader.h / Shader.cpp
  Texture.h/ Texture.cpp
  Vector.h/ Vector.cpp
  SphericalCameraManipulator.h / SphericalCameraManipulator.cpp

models/
  chassis.obj  turret.obj  front_wheel.obj  back_wheel.obj
  cube.obj     coin.obj    ball.obj
  hamvee.bmp   Crate.bmp   coin.bmp         ball.bmp
  Map1.txt     Map2.txt    Map3.txt

main.cpp
shader.vert
shader.frag
qmake.pro
README.md
```
---

## Build (Linux, qmake)
**Dependencies** (from `qmake.pro` and includes):
- OpenGL (`GL`), GLU (`GLU`), GLEW (`GLEW`), GLUT (`glut`)
- C++ compiler compatible with GLSL `#version 120`

**Steps:**
```bash
qmake
make
```
Linked libraries (as per `qmake.pro`):
```
-lGLEW -lglut -lGLU -lGL
```

---

## Run
Once built, launch the executable. Level-specific time limits and map files are set in `Level()`:
- Level 1 → `./models/Map1.txt`, **30.0 s**
- Level 2 → `./models/Map2.txt`, **80.0 s**
- Level 3 → `./models/Map3.txt`, **150.0 s**

HUD displays **Time**, **Coins Collected**, and **Level**. Status text appears when the level is complete or time runs out.

---

## Demo
![Gameplay demo](https://github.com/user-attachments/assets/b7968c2d-a914-46dd-be7b-e0c48eb0a348)

---

## Module Header
**EEE2041 — COMPUTER VISION & GRAPHICS**  
**Assignment:** Tank Game (OpenGL/GLUT)  
**Student:** Jana Abou Fakher (ID: 6655682), Academic Year: 2022–23

---

## License
Copyright (c) 2023 Jana Abou Fakher

This project was created as part of **EEE2041 – Computer Vision & Graphics** at the **University of Surrey**.  
The author retains copyright. The University of Surrey holds a non-exclusive license to use this work for teaching and assessment purposes.  

All other rights reserved. Redistribution or use outside of the University context requires the author’s permission.
