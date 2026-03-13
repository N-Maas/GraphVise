# GraphVise

![Test Image 1](graph.png)

GraphVise is a simple Graph Visualization Tool built in OpenGL. It takes graphs as input in form 
of a special .txt format or as a .cnf file. Additionally, another .txt file can be provided to 
subdivide the graph into groups of the same color. 

## Features

> **Graph Import:** Either .txt in the following format:
> > v=7, e=7   
> 0 2   
> 0 3  
> 1 6  
> 3 4  
> 3 5  
> 3 6  
> 5 1
> 
> Where v is the number of vertices and e is the number of edges.
> 
> or a .cnf file in the following format:
> > p cnf 7 7   
> 1 -2 0   
> -1 -2 0   
> -1 3 0   
> 3 -4 0   
> 3 -5 0   
> 3 -6 0   
> 5 -1 0

> **Graph Export:** A .png of the loaded graph can be exported.

> **Graph Manipulation:** Graphs can be subdivided into groups, which can be colored individually or hidden.
> The visual size of vertices/edges can be changed.

**General Features**
> Camera movement:  
> can be toggled between free movement and rotation around the 
> center point of the graph or a vertex. 

> Light Source Behavior:    
> can be toggled between being connected to the camera position or the center of mass of the graph
>

> Camera Bookmarks:     
> store current position of camera to go back to later
>

> Egde/Vertex Searches:     
> by typing the edge or vertex id into a search field the user can see the edge or vertex highlighted in the graph
>  

> Edge/Vertex Info:     
> by clicking on an edge/vertex a window opens containing information about the clicked edge/vertex
> 

## Dependencies

The project is defined using [CMake](https://cmake.org/) and uses

* [GLFW](https://www.glfw.org/) to create windows and rendering surfaces
* [Glad](https://github.com/Dav1dde/glad) for providing an OpenGL 3.3 Core profile
* [Dear Imgui](https://github.com/ocornut/imgui) for the graphical user interface (GUI)
* [OpenGL](https://www.opengl.org) as the graphics API
* [glm](https://github.com/g-truc/glm) as OpenGL Mathematics library for vector and matrix operations
* [wembed]() graph embedding library
* [Boost]() extending functionality of C++ Standard Library
* [Eigen3]() C++ template library for linear algebra

## Installation Instructions

Glad and Imgui are directly included as source files in the `ext/` directory and build at compile time. 
Additionally, wembed and platform_folders are provided as fetched content.
For the rest of the dependencies, you have to provide the libraries through local installations:

### Ubuntu / Debian / Unix

All libraries can be installed from the package manager. Open a terminal and install

`sudo apt install build-essential cmake libgl1-mesa-dev libglfw3-dev libglm-dev libboost-all-dev libeigen3-dev`

If you want to configure the CMake project with a GUI application you can optionally add

`sudo apt install cmake-qt-gui`


### WindowsGL

ToDo: Install CMake, GLFW, CMake, glm, boost, Eigen3 and OpenGL libraries.


## Build Instructions

You can build the project from the console or alternatively use an IDE for developing and building:

### From the Console 

One advantage of CMake are out of source builds, i.e. creating all build files in a separate directory. Since this is good practice we create a build sub-directory, use CMake to create our make file and then build the project with make:

```
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

You can then run the application from the build directory with

`./GraphVise`

### Using an IDE

You can import the CMakeLists.txt file from the base directory in an IDE that supports CMake, for example [CLion](https://www.jetbrains.com/de-de/clion/) or [Visual Studio](https://visualstudio.microsoft.com/de/free-developer-offers/).
If you are using Visual Studio you can use the CMake GUI program to create Visual Studio project files from the `CMakeLists.txt`.


### OpenGL Versions and Profiles
If you have to use another OpenGL version instead of the 3.3 Core profile or need any extensions, you can create the Glad [online loader generator](https://glad.dav1d.de/) to create a suitable Glad loader.
Just replace the loader in ext/glad with the new files and recompile the project.
