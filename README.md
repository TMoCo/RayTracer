# Raytracing application

## Motivation
After writing a simple raytracer for my 4th year rendering module, I've decided to revisit raytracing and learn more about this rendering paradigm. My previous application, although sufficient to get a good mark, could have gone a lot further. With this project, I want to improve on the aspects of the coursework I found lacking. 
The main issues with the coursework was speed: rendering an image was a slow, painful and soul crushing experience, even for a small number of primitives. Another issue was sampling and scattering which generated bad random light bounce reflections. Finally, building scene was too complicated as scene geometry and material information had to be merged into a complicated file extending the wavefront OBJ format; Needless to say, it wasn't pretty. This project aims to adress these issues whilst exploring ray tracing further in the hope to render some impressive scenes and experiment with more advanced concepts such as acceleration data structures and volumetric rendering.

## The Initial plan
The initial plan for the project is to address the issues present in the previous application. This requires:
- [x] faster mathematics
- [x] a better material system for PBR materials (ie textures)
- [x] a simpler scene file format 

## Moving forward
After generating some images, the next steps of the project are:
- [ ] Integrate triangle meshes into the scene file format
- [ ] Render a debug scene in OpenGl (view BVH and help position camera)
- [ ] A material file format for PBR 
- [ ] More accurate Monte Carlo sampling/scattering
- [ ] Volumetric rendering
- [ ] Bounding volume hierarchies

## Setbacks
My initial coursework was written for Linux, and I started this project on the same platform. As the application grew in size, I quickly realised I needed a better development environment, warranting a switch to Windows and Visual Studio. I used the switch as a pretext to use a more recent OpenGL version (from 1.1 to 4.5) and jump from using Qt to ImGUI, which I will integrate into the application later as a replacement to the Qt Widgets. Before implementing these upgrades, I needed to generate a visual studio solution and manage the different APIs used. To do so, I learnt to use CMake which I now use to generate visual studio solutions for the project.

## Screenshots
A selection of screen shots taken whilst developing the applicaiton:
![normals](https://user-images.githubusercontent.com/56483943/147217267-d7b643c7-b6c9-4ed0-8c6b-c4972e3f656c.jpg)

Sphere Normals

![ThreeBalls](https://user-images.githubusercontent.com/56483943/147217290-835f5955-d1bb-4ccd-8114-4df7b90fcc83.jpg)

A fuzzy metallic ball, a diffuse ball and a glass ball

![Earth](https://user-images.githubusercontent.com/56483943/147217986-029a85b9-8209-44db-a718-174cd7bbe44b.jpg)

Planet earth

![GlassBallAndLight1000Samples](https://user-images.githubusercontent.com/56483943/147218504-a9d69c80-9ba2-4620-ad80-980de5511340.jpg)

A glass ball illuminated by a ball of light, look at those caustics!

## Desirable features
- [ ] acceleration data structures
- [ ] threading
- [ ] refractive materials
- [ ] subsurface scattering / volumetric rendering
- [ ] instanced geometry
- [ ] atmospheric effects (fog, clouds)

## Link to main resources used:
- [Physically Based Rendering](https://www.pbr-book.org/3ed-2018/contents) - Goes into a lot of detail explaining the theory behind PBR whilst also providing a lot of code with the PBRT code base.
- [Ray Tracing in one weekend books](https://raytracing.github.io/) - Much more palatable than the PBR book, provides source code and instructions for building a ray tracer.

## Libraries/APIs used
- OpenGL 
- glfw window API
- glad
- stb_image
- thomath (my maths!)
