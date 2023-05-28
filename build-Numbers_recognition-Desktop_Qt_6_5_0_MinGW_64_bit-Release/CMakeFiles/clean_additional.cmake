# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Numbers_recognition_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Numbers_recognition_autogen.dir\\ParseCache.txt"
  "Numbers_recognition_autogen"
  )
endif()
