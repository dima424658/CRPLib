cmake_minimum_required(VERSION 3.7)

project(CRPLib)

set(CRP_SOURCES 
    ${CMAKE_SOURCE_DIR}/Sources/Base.cpp
    ${CMAKE_SOURCE_DIR}/Sources/BPlanes.cpp
    ${CMAKE_SOURCE_DIR}/Sources/CrpFile.cpp
    ${CMAKE_SOURCE_DIR}/Sources/DataAllocator.cpp
    ${CMAKE_SOURCE_DIR}/Sources/Effect.cpp
    ${CMAKE_SOURCE_DIR}/Sources/Entry.cpp
    ${CMAKE_SOURCE_DIR}/Sources/Material.cpp
    ${CMAKE_SOURCE_DIR}/Sources/Matrix.cpp
    ${CMAKE_SOURCE_DIR}/Sources/Part.cpp
    ${CMAKE_SOURCE_DIR}/Sources/RawData.cpp
    ${CMAKE_SOURCE_DIR}/Sources/Vector2.cpp
    ${CMAKE_SOURCE_DIR}/Sources/Vector4.cpp)
    
add_library(${PROJECT_NAME} STATIC ${CRP_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Include)