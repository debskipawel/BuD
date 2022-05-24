set(BIN_NAME MGSandbox)

add_executable(${BIN_NAME} main.cpp)

# Includowanie output dira, aby uzyskac potzrebne headery z Serializera
# Jak ktos będzie miec lepszy pomysł jak poprawic zależności pomiędzy Sandboxem a Serializerem to prosze bardzo
target_include_directories(${BIN_NAME} PRIVATE
        ${PROJECT_BINARY_DIR}/include/Serializer
        )

target_link_libraries(${BIN_NAME} PRIVATE MGSerializer)

# [TODO] Przy dodaniu wiekszej ilości sampli przerobić na jakąś pętlę po jsonach
add_custom_command(
        TARGET ${BIN_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/file_example.json
        ${PROJECT_BINARY_DIR}/file_example.json)

add_custom_command(
        TARGET ${BIN_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/file_example_2.json
        ${PROJECT_BINARY_DIR}/file_example_2.json)