cmake_minimum_required(VERSION 2.8)
project( QRScanner )
find_package( OpenCV REQUIRED )
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( qrs qrscanner.cpp )
target_link_libraries( qrs ${OpenCV_LIBS} )
