include(CheckIncludeFiles)
cmake_minimum_required(VERSION 3.10) # common to every CLion project
project(OpenGLLinuxTest) # project name

if (UNIX)
    set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable.
    ################################################
    add_executable(OpenGLLinuxTest1 SBE306B_Spring20_Project_Group_.cpp startRendering.cpp texture.cpp imageLoader/imageloader.cpp) #common to all clion project
    target_link_libraries(OpenGLLinuxTest1 ${OpenGlLinkers}) # linking opengl libraries to the project
    #################################################
endif ()

if (WIN32)
    set(CMAKE_CXX_STANDARD 11)
    include_directories(OpenGL)
    include_directories(OpenGL/include) # OpenGL/include has to contain the required OpenGL's .h files
    include_directories(OpenGL/lib) # OpenGL/lib has to contain the required OpenGL's .lib files
    # glut32.dll must be present in "project-directory/OpenGL/dll/"
    add_custom_target(glutdlllib
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/OpenGL/dll/glut32.dll ${CMAKE_BINARY_DIR}
            )
    # required .lib files to be copied into compiler's proper directory
    set(OpenGlLibs glaux glu32 glui32 glut32 opengl32)
    #These 3 lines are just linking and making executables
    add_executable(demo SBE306B_Spring20_Project_Group_.cpp startRendering.cpp texture.cpp imageLoader/imageloader.cpp)
    target_link_libraries(demo ${OpenGlLibs}) #linking the libraries to the project file
    add_dependencies(demo glutdlllib) #add dll files as dependencies
endif ()
