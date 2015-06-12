add_executable(marker_ar_demo 
ARDrawingContext.hpp
ARDrawingContext.cpp
BGRAVideoFrame.h
CameraCalibration.cpp
CameraCalibration.hpp
GeometryTypes.cpp
GeometryTypes.hpp
main.cpp
Marker.cpp
Marker.hpp
MarkerDetectionFacade.hpp
MarkerDetector.cpp
MarkerDetector.hpp
TinyLA.cpp
TinyLA.hpp
)

target_link_libraries( marker_ar_demo ${OpenCV_LIBRARIES} )
target_link_libraries( marker_ar_demo ${OPENGL_LIBRARIES} )
     
install (TARGETS marker_ar_demo DESTINATION bin)
