# Gazel

![Presentation Image](saved/examples/example.png)

# The Goal

This Tool Is A Rendering Engine Who's Output Is A Video Clip. This Tool Is A Simplistic Attempt At Recreating The Software Used In 3D Animated Movies.

## Requirements

C++, Python, and the cv2 python package.

## Features

+ Phong Rendering
+ Gouraud Rendering
+ OBJ File Parsing
+ Diffuse Lighting
+ Specular Lighting
+ Material System
+ Object Rotation
+ Camera Movement
+ Camera Rotation


## How To Install

[Install Python](https://www.python.org/downloads/)

```
git clone https://github.com/PolarToCartesian/Render-Farm
cd Render-Farm/setup/
python install_python_dependencies_pip.py
```

Install a ppm image viewer if you want to look at individual frames.

## Compiling On Linux

```
cd Render-Farm/setup/
python linux_compile.py
```

Note: G++ Warnings Are Disabled.

The executable will be located in the main Render-Farm/ directory.

## Thanks To

### Documentation / Resources

+ [Barycentric Interpolation](https://codeplea.com/triangular-interpolation)
+ [Surface Normal Calculation](https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal)
+ [General 3D Matrix Calculations](https://www.youtube.com/playlist?list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP)
+ [2D Triangle Renderer](https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling)
+ [Specular Lighting / Texturing / Phong Shading](https://www.youtube.com/watch?v=Is6D5rnWEvs&list=PL_w_qWAQZtAZhtzPI5pkAtcUVgmzdAP8g&index=11&t=1512s)
+ [Calculating The Reflexion Vector](https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector)
+ [PPM 3 And 6 File Format Specification](http://netpbm.sourceforge.net/doc/ppm.html)

### Test Models

+ [town.txt](https://free3d.com/3d-model/environment-23742.html)
+ [quad.txt](https://github.com/triplepointfive/ogldev/blob/master/Content/quad.obj)
+ [teapot.txt](https://graphics.stanford.edu/courses/cs148-10-summer/as3/code/as3/teapot.obj)
+ [armadillo.txt](http://www.prinmath.com/csci5229/OBJ/index.html)