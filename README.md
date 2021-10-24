# Raytracing application
## Motivation
After writing a simple raytracer for my 4th year rendering module, I've decided to revisit raytracing and learn more about this rendering paradigm. My previous application, although sufficient to get a good mark, could have been better. With this project, I want to improve on the aspects of the coursework I found lacking. 
The main issues with the coursework were the speed. Rendering an image was a slow, painful and a soul crushing experience. Another issue was sampling. Equally slow, generating random light bounce reflections produced false results. Finally, scene building was too complicated. Scene geometry and material information had to be merged into a complicated file in the same format as wavefront OBJ. Needless to say, it wasn't pretty.
## The plan
The plan for this application, for now, is to address the issues present in the previous application. This requires:
-[x] better and faster mathematics 
-[x] correct hemisphere sampling 
-[ ] correct lighting
-[ ] generate some nice images
-[ ] better materials
-[ ] simple scene data structure

## So far
The first thing to do is to adress the issues in the old application. My initial coursework was written for Linux, and I started this project on the same platform. As the application grew, I realised I needed a better development environment, which warranted a switch to Windows and Visual Studio. The switch meant I could no longer use the Qt gui API. I have experience using ImGUI, which I will integrate into the application later. 
