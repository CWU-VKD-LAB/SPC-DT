cmake_minimum_required(VERSION 3.21)
project(OpenGL_on_a_Windows_Form)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Debug)
include_directories(Release)

add_executable(OpenGL_on_a_Windows_Form
        Debug/OpenGL_on_a_Windows_Form.pch
        Release/OpenGL_on_a_Windows_Form.pch
        AssemblyInfo.cpp
        ClassData.h
        CombinedCPC.cpp
        CombinedCPC.h
        Cont_Parallel_Coordinates.cpp
        Cont_Parallel_Coordinates.h
        Dist_Cont_Parallel_Coords.cpp
        Dist_Cont_Parallel_Coords.h
        FileHandling.cpp
        FileHandling.h
        Form1.h
        glut.h
        InteractiveSPC.cpp
        InteractiveSPC.h
        OpenGL3.h
        OpenGL_on_a_Windows_Form.cpp
        resource.h
        SeparatedCPC.cpp
        SeparatedCPC.h
        SeparatedSPC.cpp
        SeparatedSPC.h
        SeparatedTS_SPC.cpp
        SeparatedTS_SPC.h
        StandardTS.cpp
        StandardTS.h
        stdafx.cpp
        stdafx.h
        TimeSeries_Dist.cpp
        TimeSeries_Dist.h)
