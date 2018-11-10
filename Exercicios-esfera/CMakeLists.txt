cmake_minimum_required(VERSION 3.4)

project(Exercicios_Esfera)

SET(CMAKE_C_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Exercicios_Esfera MovimentoEsfera.cpp funcoes.h particula.h)

target_link_libraries(Exercicios_Esfera -static-libgcc -static-libstdc++ -lm)