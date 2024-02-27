# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Licenta_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Licenta_autogen.dir\\ParseCache.txt"
  "Licenta_autogen"
  )
endif()
