# CMakeList.txt : CMake project for expected, include source and define
# project specific logic here.
#

add_library(out_ptr 
"include/out_ptr.h" "out_ptr.cpp" )

target_include_directories(out_ptr PUBLIC include)

set_target_properties(out_ptr PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(out_ptr PRIVATE cxx_std_20)
