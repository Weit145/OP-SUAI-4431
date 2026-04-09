cmake_minimum_required(VERSION 3.10)   
project(AL_kurs)                       
# Указываем, что хотим собрать исполняемый файл "main" из двух исходников
add_executable(main main.cpp variant/variant.cpp tree/tree.cpp hash/hash.cpp domain/domain.cpp transaction/transaction.cpp print/print.cpp)
