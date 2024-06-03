# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CppTask4GraphLayers_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CppTask4GraphLayers_autogen.dir\\ParseCache.txt"
  "CppTask4GraphLayers_autogen"
  )
endif()
