cmake_minimum_required(VERSION 3.28.3)
project(Radioactive_Decay_Calculator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Python3 COMPONENTS Interpreter Development Numpy)
Include_directories(${Python3_INCLUDE_DIRS} 
                    ${Python3_NumPy_INCLUDE_DIRS}
                    /home/hridoy/miniconda3/lib/python3.13/site-packages/numpy/_core/include 
                    include
                    )
add_executable(Radioactive_deacy_calculator src/Radioactive_deacy_calculator.cpp)
target_link_libraries(Radioactive_deacy_calculator ${Python3_LIBRARIES})

# Prefer static libstdc++/libgcc to avoid environment-provided incompatible runtimes (e.g., Conda)
target_link_options(Radioactive_deacy_calculator PRIVATE -static-libstdc++ -static-libgcc)

# Ensure runtime data (CSV) is available next to the built executable
configure_file(${CMAKE_SOURCE_DIR}/src/half_life.csv
               ${CMAKE_CURRENT_BINARY_DIR}/half_life.csv COPYONLY)
