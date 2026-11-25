add_executable(task02_tests
    test_models.cpp
    test_signals_simulation.cpp
    test_stability.cpp
)

target_link_libraries(task02_tests PRIVATE task02_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(task02_tests)
