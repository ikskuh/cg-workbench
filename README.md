# CG Workbench
The CG Workbench is a small GLSL coding environment which allows to rapidly prototype some shaders or shader pipelines.

![](https://mq32.de/public/2801bead79cd300b41f6e983d8e4a7fbd7b134dc.png)

Live Shading Demo: [Youtube](https://www.youtube.com/watch?v=et6Kq0yJqxU)

## Controls / Usage

| Input | Action |
|-|-|
| Ctrl+Space, Rightclick on Background | Open *New Node* window |
| Right-Click on a node | Open node editor (save to template, rename node) |
| Ctrl + Left Mouse + Drag | Drag viewport window |
| Ctrl + R | Recompile current window |
| Ctrl + S | Save |
| Ctrl + Shift + S | Save As |
| Ctrl + N | New document |
| Ctrl + O | Open File |
| Alt + F4 | Exit |

## Build Instructions

CG Workbench uses [Zig](https://ziglang.org/) as a build system. Just call `zig build` to build your project, `zig-cache/bin` will contain the executable.

### Linux
Install the following libraries / packages:
- `sdl2`
- `gtk-3.0`

### Windows
Should work out of the box

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
- Save query when creating new node with existing content
- Shader Common Node
  - Provide "includes" for all shaders
- Graphic Nodes
	- Geometry Node
		- Primitives
			- Sphere
			- Cube
- Player-Modus (lädt jgraph-File und hat ein Haupt-Renderer)
- Event-Delay Visualisierung
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
- Synth-Kram mit T$
- LUT-Node
	- Maps input→output by indexing a table
- Audio Nodes
	- Signal Mixer Node
	- Envelope Generator Node