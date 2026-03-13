# C++ Game Kit
A bare-bones C++ framework for game development.  
  
<img width="1280" height="750" alt="screenshot" src="https://github.com/user-attachments/assets/4e50a2ef-691c-4ba0-85ea-949875abaea0" />

## Supported Platforms
* Windows 10 x64
* macOS arm64

## Building
* Requires Python to be installed
* Use CMake to build
* On macOS, make sure the ```Data``` is inside the app bundles ```Resources``` folder  
  
Preferably, use VS2022 or XCode.  
  
## Features
* <b>Core</b>
  * INI file reading/writing
  * Simple binary archive
  * Alarm event system
  * Fixed-timestep clock
  * Simple smart pointer
* <b>OS</b>
  * Window management
  * Keyboard, mouse and gamepad input
* <b>Networking</b>
  * TCP Server/Client
  * UDP Server/Client
* <b>Math</b>
  * Vector 2D
  * Vector 3D
  * Vector 4D
  * Matrix 4x4
  * Quaternion
  * Transformation
  * 2D Collision
    * AABB
    * Circle
  * 3D Collision
    * AABB
    * Sphere
* <b>Rendering</b>
  * Shader manager
  * Framebuffer manager
  * Out-of-the-box 2D rendering pipeline
    * Sprite rendering
    * Sprite atlas rendering
    * Sprite sheet animation
    * Sprite font rendering
    * Hardware instancing
  * Automatic Texture Mipmap Generation
  * 3D Model loading
  * 3D Scene setup
  * Custom GPU-Friendly 3D Model Format

## Third-party libraries used:
* [stb_image](https://github.com/nothings/stb)
* [SoLoud](https://solhsa.com/soloud/)
* [SDL3](https://github.com/libsdl-org/SDL)
* [bgfx](https://github.com/bkaradzic/bgfx)
* [assimp](https://github.com/assimp/assimp)
