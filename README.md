# CG Workbench
The CG Workbench is a small GLSL coding environment which allows to rapidly prototype some shaders or shader pipelines.

![](https://puu.sh/yOGv5/6c440d280e.png)

## Build Instructions

CG Workbench uses qmake as a build system, but doesn't require Qt.

Instead of executing qmake manually, QtCreator can be used to build the project.

### Linux
Install the following libraries / packages:
- `sdl2`
- `glm`
- `qt-5` or `qmake`
- `lua` / `lua-5.3`
- `gtk-3.0`

Create a folder `build/`, enter it and run qmake with the `cg-workbench.pro` file specified
as an argument. Then run `make`.

### Windows

Create a folder `lib` with the following contents and the corresponding libraries included.

```
cg-workbench/lib
├───glm
│   ├───glm
│   │   ├───detail
│   │   ├───gtc
│   │   ├───gtx
│   │   └───simd
├───lua-5.3.4
│   └───include
├───nfd
└───SDL2-2.0.7
    ├───include
    └───lib
        └───x64
```

Then use QtCreator to build the project.

## External Libraries
This project would be much harder without those great libraries:
- [SDL2](https://www.libsdl.org)
- [GLM](https://glm.g-truc.net/0.9.8/index.html)
- [github.com/nlohmann/json](https://github.com/nlohmann/json)
- [github.com/skaslev/gl3w](https://github.com/skaslev/gl3w)
- [github.com/ocornut/imgui](https://github.com/ocornut/imgui)
- [github.com/mlabbe/nativefiledialog](https://github.com/mlabbe/nativefiledialog)
- [github.com/nothings/stb](https://github.com/nothings/stb)
- [github.com/syoyo/tinyobjloader](https://github.com/syoyo/tinyobjloader)
- [github.com/cxong/tinydir](https://github.com/cxong/tinydir)
- [github.com/severin-lemaignan/webcam-v4l2](https://github.com/severin-lemaignan/webcam-v4l2)

## Ideas / Missing Nodes
- Graphic Nodes
	- Geometry Node
		- Primitives
			- Sphere
			- Cube
- Buffer -> Texture
	- 1D Input/Over → 1D Texture with *n* width
	- 2 * 1D Input/Over → 2D Texture with *n* * *m* width
- Input Node
	- Keyboard
	- Mouse
	- Joystick
- Lua Node
	- SetUp
	- Update
	- Shutdown
- Sound Node
	- Seek (?)
- Timeline Node
	- Attach values to a timeline
	- Time as input
- LUT-Node
	- Maps input→output by indexing a table
- Audio Nodes
	- Signal Mixer Node
	- Envelope Generator Node