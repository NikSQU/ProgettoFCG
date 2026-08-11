cmake_minimum_required(VERSION 3.16)
project(Mulino3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- GESTIONE AUTOMATICA SFML 3 e GLM TRAMITE FETCHCONTENT ---
include(FetchContent)

# 1. Scarica SFML 3.0
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
)
FetchContent_MakeAvailable(SFML)

# 2. Scarica GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# --- PACCHETTI DI SISTEMA ---
find_package(OpenGL REQUIRED)

# Include directories per GLAD e i tuoi header locali
include_directories(glad include)

# --- TARGET PRINCIPALE ---
add_executable(Mulino3D
    main.cc
)

# Collegamento delle librerie
target_link_libraries(Mulino3D PRIVATE SFML::Graphics OpenGL::GL glm::glm ${CMAKE_DL_LIBS})