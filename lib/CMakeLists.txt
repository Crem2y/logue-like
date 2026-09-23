add_subdirectory(logue_core)
add_subdirectory(logue_generator)
add_subdirectory(logue_renderer)


add_library(lib INTERFACE)

target_link_libraries(lib INTERFACE
    logue_core
    logue_generator
    logue_renderer
)