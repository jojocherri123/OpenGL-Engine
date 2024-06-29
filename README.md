###### This is my attempt at making an OpenGL engine with C++

press M to free and unfree camera

#### libraries required:
- SDL2
  - sudo apt install libsdl2-dev
- glm
  - sudo apt install libglm-dev
- assimp
  - sudo apt install libassimp-dev
- stb
  - sudo apt install libstb-dev

##### tested on Linux Ubuntu Budgie

it is strictly still a WIP
=

lights
=

there are 3 types of lights currently
- pointLight
- directionalLight
- spotlight

all of which can be accesed currently through fragment.glsl shader 

importing models
=
This engine currently supports .obj only, more to come in the future 

Menus
=
menus currently only edit lights (WIP)