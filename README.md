# VcTest
My voxel cone tracing test in the sponza scene. Tested on debian 9 with Nvidia Quadro M1200. Uses about 1130MB video memory (for 512x256x256 voxels)
## Dependencies
OpenGL(>=4.5), [glfw(3.2.1)](http://www.glfw.org/), [stb_image.h](https://github.com/nothings/stb), [gl3w](https://github.com/skaslev/gl3w), [tinyobjloader](https://github.com/syoyo/tinyobjloader)
## Build
```bash
git clone https://github.com/AdamYuan/VcTest.git
cmake . -DCMAKE_BUILD_TYPE=Release
make
```
## Usage
* W, A, S, D, L-Shift, Space: Move around
* Esc: Toggle window focus
* C: Toggle albedo
* V: Toggle voxel cone tracing
* E: Toggle showing edge 
* X: Debug voxel
## Screenshots
![alt text](https://raw.githubusercontent.com/AdamYuan/VcTest/master/screenshots/1.png)
![alt text](https://raw.githubusercontent.com/AdamYuan/VcTest/master/screenshots/2.png)
![alt text](https://raw.githubusercontent.com/AdamYuan/VcTest/master/screenshots/3.png)
## References
* https://jose-villegas.github.io/post/deferred_voxel_shading/
* http://simonstechblog.blogspot.com/2013/01/implementing-voxel-cone-tracing.html
* http://www.seas.upenn.edu/~pcozzi/OpenGLInsights/OpenGLInsights-SparseVoxelization.pdf
