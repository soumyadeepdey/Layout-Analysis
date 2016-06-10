cmake_minimum_required(VERSION 2.8)
project( PARA )
set(BUILD_SHARED_LIBS OFF)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package( OpenCV REQUIRED )

SET(PARA_SOURCES  ParagraphDetection.cpp 
binarization.cpp
connectedcomponent.cpp
GapfillingSmoothing.cpp
ImageProcFunctions.cpp
morphology.cpp
PixelValidation.cpp
RectangleTest.cpp
schemaParser.cpp
xml.cpp
xmlParser.cpp
)
SET(PARA_HEADERS myheader.h xmlParser)  


# Or set it globally for all targets:
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

ADD_EXECUTABLE(para ${PARA_SOURCES} ${PARA_HEADERS} )
TARGET_LINK_LIBRARIES(para ${OpenCV_LIBS})
set(CMAKE_BUILD_TYPE Debug)

