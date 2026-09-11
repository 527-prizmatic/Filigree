add_executable(${PROJECT_NAME} "main.cpp")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-system sfml-window sfml-audio)
target_link_libraries(${PROJECT_NAME} PRIVATE GSL)
# target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann::json)

# set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)
# set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD_MODE BATCH)

target_precompile_headers(${PROJECT_NAME} PRIVATE "include/Iridium/libraries.hpp")
target_include_directories(${PROJECT_NAME} PUBLIC "include" "include/Iridium")

# PROJECT CONFIG BELOW THIS LINE
