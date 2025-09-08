# EEE2041 — COMPUTER VISION & GRAPHICS
## Tank Game (OpenGL/GLUT)

A small tank game implemented with OpenGL (GLSL) and GLUT, created for **EEE2041 — COMPUTER VISION & GRAPHICS**.

> **Status:** This repository reflects the **current** state of the project as provided. Several required source files and all runtime models/textures are **missing**; see **Missing Files** and **Assets**.

---

## Features (from code)
- **GLSL pipeline**
  - Vertex shader: `shader.vert` (attributes: `aVertexPosition`, `aVertexTexcoord`; uniforms: `MVMatrix_uniform`, `ProjMatrix_uniform`)
  - Fragment shader: `shader.frag` (uniform: `TextureMap_uniform`)
  - Shaders loaded via `Shader::LoadFromFile("shader.vert","shader.frag")`.
- **Geometry & Textures**
  - OBJ mesh loading/drawing via `common/Mesh.{h,cpp}`
  - BMP texture loading via `Texture::LoadBMP` (header referenced but not present).
- **Math Utilities**
  - 4×4 matrix operations via `common/Matrix.{h,cpp}` for model–view and projection transforms.
- **Camera**
  - Spherical camera manipulator (`SphericalCameraManipulator`) for mouse-driven pan (cursor hidden in window).
- **Gameplay (from `main.cpp`)**
  - Tank with chassis, turret, and wheels; turret pans with mouse.
  - Fire a ball from the turret; simple range/interaction checks.
  - Collectible coins; per-level time limit; “Level Completed” / “Out of Time” states.
  - Middle-click context menu: **Pause**, **Continue**, **Exit**.
  - Levels loaded from text files (`Map1.txt`, `Map2.txt`, `Map3.txt`).

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
  Matrix.h / Matrix.cpp   # 4×4 matrices (translate/rotate/scale/perspective/lookAt)
  Mesh.h   / Mesh.cpp     # OBJ parsing, GL buffers, Draw()

main.cpp                  # init, input, rendering, gameplay, HUD, level loading
shader.vert               # GLSL vertex shader
shader.frag               # GLSL fragment shader
qmake.pro                 # qmake project file (Linux, OpenGL/GLUT/GLEW/GLU)
```
> Note: `common.zip` also contained `Matrix` and `Mesh`. The repository *references* additional files listed below that are **not present**.

---

## Missing Files (required to build)
The following are included in the code and qmake project but **not present** in this repo snapshot:
- `common/Shader.h`, `common/Shader.cpp` (for `Shader::LoadFromFile`)
- `common/Vector.h` (and possibly `Vector.cpp`)
- `common/Texture.h`, `common/Texture.cpp` (for `Texture::LoadBMP`)
- `common/SphericalCameraManipulator.h`, `common/SphericalCameraManipulator.cpp`

> Without these, the project will not compile.

**TODO:** Add the missing headers/implementations to `common/`.

---

## Assets (required at runtime)
The game loads models, textures, and maps from **`../models/`** (relative to the executable’s working directory). Filenames referenced in `main.cpp` include:
- **Meshes:** `chassis.obj`, `turret.obj`, `front_wheel.obj`, `back_wheel.obj`, `cube.obj`, `coin.obj`, `ball.obj`
- **Textures:** `hamvee.bmp`, `Crate.bmp`, `coin.bmp`, `ball.bmp`
- **Level maps:** `Map1.txt`, `Map2.txt`, `Map3.txt`

> These assets were only on the original machine and are **currently unavailable**. The game will compile (once sources are complete) but cannot run correctly without them.

**TODO:** Restore/populate `../models/` with the exact files listed above.

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

> Ensure the **missing source files** are added to `common/` and library paths resolve on your Linux system.

---

## Run
Once built, launch the executable. Level-specific time limits and map files are set in `Level()`:
- Level 1 → `../models/Map1.txt`, **30.0 s**
- Level 2 → `../models/Map2.txt`, **80.0 s**
- Level 3 → `../models/Map3.txt`, **150.0 s**

HUD displays **Time**, **Coins Collected**, and **Level**. Status text appears when the level is complete or time runs out.

> **Assets required:** The executable expects the `../models/` folder populated as above.

---

## Demo
A gameplay video exists.

**TODO:** Add a link to the video or place the file at `docs/demo.mp4` and link it here.

---

## Module Header
**EEE2041 — COMPUTER VISION & GRAPHICS**  
**Assignment:** Tank Game (OpenGL/GLUT)  
**Student:** Jana Abou Fakher 2023

---

## License
Copyright (c) 2023 Jana Abou Fakher

This project was created as part of EEE2041 – Computer Vision & Graphics at the University of Surrey.
The author retains copyright. The University of Surrey holds a non-exclusive license to use this work for teaching and assessment purposes.

All other rights reserved. Redistribution or use outside of the University context requires the author’s permission.
