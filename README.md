# CG Workbench
The CG Workbench is a small GLSL coding environment which allows to rapidly prototype some shaders or shader pipelines.

![](https://puu.sh/yOGv5/6c440d280e.png)

## External Libraries
This project would be much harder without those great libraries:
- [SDL2](https://www.libsdl.org)
- [github.com/nlohmann/json](https://github.com/nlohmann/json)
- [github.com/skaslev/gl3w](https://github.com/skaslev/gl3w)
- [github.com/ocornut/imgui](https://github.com/ocornut/imgui)
- [github.com/mlabbe/nativefiledialog](https://github.com/mlabbe/nativefiledialog)
- [github.com/nothings/stb](https://github.com/nothings/stb)
- [github.com/syoyo/tinyobjloader](https://github.com/syoyo/tinyobjloader)
- [github.com/cxong/tinydir](https://github.com/cxong/tinydir)

## Ideas / Missing Nodes
- Geometry Node
	- Primitives
		- Quad
		- Sphere
		- Cube
- Event Nodes
	- Counter Node
	- 
- Noise Node
	- Output: Value, 1D Image, 2D Image, 3D Texture
- Buffer -> Texture
	- 1D Input/Over → 1D Texture with *n* width
	- 2 * 1D Input/Over → 2D Texture with *n* * *m* width
- Music Node
	- Seek
- Timeline Node
	- Attach values to a timeline
	- Time as input