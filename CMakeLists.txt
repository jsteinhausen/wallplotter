cmake_minimum_required(VERSION 3.23)
project(wallplotter)

set(CMAKE_CXX_STANDARD 14)

add_executable(wallplotter
        platform/arduinomega/main.cpp
        platform/esp32/src/main.cpp
        platform/esp32/src/servoOff.cpp
        platform/esp32/src/servoOn.cpp
        platform/esp32/src/startFunction.cpp
        platform/esp32/src/stateFunction.cpp
        platform/esp32/src/stepperController.cpp
        platform/esp32/src/stopFunction.cpp)
