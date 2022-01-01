# Raytracing application

## Motivation
After writing a simple raytracer for my 4th year rendering module, I have decided to revisit raytracing and learn more about this beautiful rendering paradigm. My previous application, although sufficient to get a good mark, could have gone a **lot** further. The main issue with the coursework was speed: rendering an image was a slow, painful and soul crushing experience, even for a small amount of geometry. Another issue was sampling and scattering which generated inacurrate ray directions. Finally, building a scene was too very clunky as scene geometry and material information had to be merged into a complicated file extending the wavefront OBJ format; Needless to say, it wasn't pretty. This project aims to adress these issues whilst exploring ray tracing further in the hope of rendering some impressive scenes and experimenting with more advanced ray tracing concepts.

## The Initial plan
The initial plan for the project is to address the issues present in the previous application. This requires:
- [x] faster and correct mathematics
- [x] a better material system (ie texture management)
- [x] a simpler scene file format

## Moving forward
After generating some images, the next steps of the project are:
- [x] Integrate triangle meshes into the scene file format
- [x] Render a debug scene in OpenGl (view BVH and help position camera)
- [x] A material file format for PBR 
- [ ] More accurate Monte Carlo sampling/scattering
- [ ] Volumetric rendering
- [x] Bounding volume hierarchies

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

![PBRv1](https://user-images.githubusercontent.com/56483943/147643686-be8e6e69-d115-48a2-9b7a-7d7aaff051ad.jpg)

Material data stored in textures. Creating complex materials based on textures is as easy as:
```
type mixture
albedoMap rustedironAlbedo.png
metallicMap rustedironMetallic.png
roughnessMap rustedironRoughness.png
normalMap rustedironNormal.png
```
![cornell box](https://user-images.githubusercontent.com/56483943/147835143-d451e774-b6c6-4b10-967d-b5cee0c00345.jpg)

Triangle meshes loaded from .obj files can also be rendered. Using Blender, I make my own models or export royalty free models from other formats such as .gltf to .obj. Scene files resemble the JSON format, for instance the cornell box scene:

```
scene cornell_box
{
  geometry box
  {
    position 0 0 -3
    rotation 0 0 0
    material cornellbox.mtl
    shape mesh cornellbox.obj
  }

  geometry light
  {
    position 0 0 -3
    rotation 0 0 0
    material light.mtl
    shape mesh cornellboxlight.obj
  }

  geometry sphere1
  {
    position 0.35 0.5 -3.65
    rotation 0 0 0
    material rustediron.mtl
    shape sphere 0.25 
  }

  geometry sphere2
  {
    position -0.55 -0.3 -3.5
    rotation 0 0 0
    material glass.mtl
    shape sphere 0.25 
  }

  geometry smallcube
  {
    position 0 0.2 -2.3
    rotation 0 0 0
    material glass.mtl
    shape mesh smallcube.obj
  }

  geometry bigcube
  {
    position -0.3 0 -3.7
    rotation 0 0 0
    material metal.mtl
    shape mesh bigcube.obj
  }
}
```
![horizontalBVH](https://user-images.githubusercontent.com/56483943/147832855-2aebf834-9f70-4e89-a28e-b687dd7b0ef1.png)
![midPointBVH](https://user-images.githubusercontent.com/56483943/147832860-fcd4f428-f13a-4626-82b1-f63fa561f858.png)

The app now has some acceleration data structures in the form of a Horizontal BVH with SAH (top) and a more basic axis midpoint separation (bottom). Notice that the density of bounding volumes is smaller and that they embrace the shape of the mesh more with the HBVH. They currently follow the implementation of the PBRT book, but I would like to add further optimisations to them when I can. 

![guiiteration4](https://user-images.githubusercontent.com/56483943/147833846-74ea9341-f0df-44fc-abca-3fe598163584.png)

Using the user interface, you can change render options and get a debug preview of the scene in opengl, as well as a view of the rendered output.

## Desirable features
- [x] acceleration data structures
- [ ] threading
- [x] refractive materials
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
