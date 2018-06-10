# VcTest
My voxel cone tracing test in the sponza scene. Tested on debian 9 with Nvidia Quadro M1200.
## Dependencies
OpenGL(>=4.5), assimp(>=3.3.1), glfw(3.2.1), stb_image.h, gl3w
## Build
```bash
git clone https://github.com/AdamYuan/VcTest.git
cmake . -DCMAKE_BUILD_TYPE=Release
make
```
## Usage
	W, A, S, D, L-Shift, Space: Move around
	Esc: Toggle window focus
	C: Toggle albedo
	V: Toggle voxel cone tracing
	E: Toggle showing edge 
## Screenshots
![alt text](https://raw.githubusercontent.com/AdamYuan/VcTest/master/screenshots/1.png)
![alt text](https://raw.githubusercontent.com/AdamYuan/VcTest/master/screenshots/2.png)
![alt text](https://raw.githubusercontent.com/AdamYuan/VcTest/master/screenshots/3.png)
