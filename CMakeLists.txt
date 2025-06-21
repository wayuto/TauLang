cmake_minimum_required(VERSION 3.31)
project(Tau C)

set(CMAKE_C_STANDARD 11)

add_executable(tau main.c
        vm.c
        parser.c
        expression.c
        statement.c
        function.c
        declaration.c
)
