# CMakeList.txt : CMake project for expected, include source and define
# project specific logic here.
#

add_library(unique_resource 
"include/unique_resource.h" "unique_resource.cpp")

target_include_directories(unique_resource PUBLIC include)

set_target_properties(unique_resource PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(unique_resource PRIVATE cxx_std_20)
