A Demo of AR(augment reality) based on maker

the code from Ch3 of the book "Mastering OpenCV with Practical Computer Vision Projects"
I modifyed it.

1. using cmake to compile it
2. need opencv/opengl library to compile
3. in line 48 main.cpp, the parameters come from the camera of my phone(HuaWei Honor 6) 
   You may need to change it to your own camera parameters.
4. the marker image is "marker.png", you can choose other marker.
	but need modify findMarkerCandidates() detectMarkers() in MarkerDector.cpp, 