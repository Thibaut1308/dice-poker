cmake_minimum_required(VERSION 3.21)
project(TP6Bonus C)

set(CMAKE_C_STANDARD 99)

add_executable(TP6Bonus archives/dice_poker.c)
add_executable(TP6Bonusv2 src/dice_poker_v2.c)