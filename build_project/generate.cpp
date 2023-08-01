#include<string>
#include<fstream>

int main(){
    std::string content="cmake_minimum_required(VERSION 3.15)\nproject(Matrix)\nget_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)\nset(LIBPATH ${PARENT_DIR}/tools/lib)\nset(CMAKE_CXX_STANDARD 11)\nset(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)\nfile(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)\ninclude_directories(${PROJECT_SOURCE_DIR}/inc)\ninclude_directories(${PARENT_DIR}/tools/inc)\nlink_directories(${LIBPATH})\nadd_executable(main ${SRC})\ntarget_link_libraries(main read)\nmessage(STATUS \"Lib Directory: ${LIBPATH}\")";
    std::ofstream ofs("CMakeLists.txt");
    ofs<<content;
    ofs.close();
    system("mkdir src inc");
    return 0;
}
