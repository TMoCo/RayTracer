# Raytracing application

## Motivation
After writing a simple raytracer for my 4th year rendering module, I've decided to revisit raytracing and learn more about this rendering paradigm. My previous application, although sufficient to get a good mark, could have gone a lot further. With this project, I want to improve on the aspects of the coursework I found lacking. 
The main issues with the coursework was speed: rendering an image was a slow, painful and soul crushing experience, even for a small number of primitives. Another issue was sampling and scattering which generated bad random light bounce reflections. Finally, building scene was too complicated as scene geometry and material information had to be merged into a complicated file extending the wavefront OBJ format; Needless to say, it wasn't pretty. This project aims to adress these issues whilst exploring ray tracing further in the hope to render some impressive scenes and experiment with more advanced concepts such as acceleration data structures and volumetric rendering.

## The plan
The initial plan for the project is to address the issues present in the previous application. This requires:
- [x] faster mathematics
- [x] a better material system for PBR materials (ie textures)
- [x] a simpler scene file format 

## Setbacks
My initial coursework was written for Linux, and I started this project on the same platform. As the application grew in size, I quickly realised I needed a better development environment, warranting a switch to Windows and Visual Studio. I used the switch as a pretext to use a more recent OpenGL version (from 1.1 to 4.5) and jump from using Qt to ImGUI, which I will integrate into the application later as a replacement to the Qt Widgets. Before implementing these upgrades, I needed to generate a visual studio solution and manage the different APIs used. To do so, I learnt to use CMake which I now use to generate visual studio solutions for the project.

## Screenshots
A selection of screen shots taken whilst developing the applicaiton
![normals](https://user-images.githubusercontent.com/56483943/147217267-d7b643c7-b6c9-4ed0-8c6b-c4972e3f656c.jpg)
![ThreeBalls](https://user-images.githubusercontent.com/56483943/147217290-835f5955-d1bb-4ccd-8114-4df7b90fcc83.jpg)
![GlassBallAndLight150samples](https://user-images.githubusercontent.com/56483943/147217320-6057c548-35d4-4da2-9ca6-9139dbfa492e.jpg)

## Desirable features
- [ ] acceleration data structures
- [ ] threading
- [ ] refractive materials
- [ ] subsurface scattering / volumetric rendering
- [ ] instanced geometry
- [ ] atmospheric effects (fog, clouds)

## Libraries/APIs used
- OpenGL 
- glfw window API
- glad
- stb_image
- thomath (my maths!)
