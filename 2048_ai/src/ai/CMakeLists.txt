add_executable(2048_ai
    main.cpp
    Heuristics.cpp
    Expectimax.cpp
)

target_include_directories(2048_ai PRIVATE
    ${CMAKE_SOURCE_DIR}/src/game/engine/src/headers
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(2048_ai PRIVATE 2048_engine)

target_compile_features(2048_ai PRIVATE cxx_std_17)
