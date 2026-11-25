add_library(task02_core
    Model15.cpp
    Model27.cpp
    Model39.cpp
    InputSignal.cpp
    Simulation.cpp
    StabilityAnalyzer.cpp
)

target_include_directories(task02_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(task_02_ii02907 main.cpp)
target_link_libraries(task_02_ii02907 PRIVATE task02_core)
