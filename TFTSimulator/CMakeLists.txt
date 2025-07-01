cmake_minimum_required(VERSION 3.16)
project(GraphicsEngine C)

# Настройки компилятора
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Поиск пакетов
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Настройка компонентов
add_executable(graphics_engine
    src/main.c
    src/fonts/font_12.c
    # src/core/graphics.c
    # src/core/fonts.c
    # src/emulator/sdl_interface.c
)

# Подключение библиотек
target_include_directories(graphics_engine PRIVATE
    ${SDL2_INCLUDE_DIRS}
    include/
)

target_link_libraries(graphics_engine
    ${SDL2_LIBRARIES}
)

if(SDL2_IMAGE_FOUND)
    target_compile_definitions(graphics_engine PRIVATE USE_SDL_IMAGE=1)
    target_link_libraries(graphics_engine ${SDL2_IMAGE_LIBRARIES})
endif()

if(SDL2_TTF_FOUND)
    target_compile_definitions(graphics_engine PRIVATE USE_SDL_TTF=1)
    target_link_libraries(graphics_engine ${SDL2_TTF_LIBRARIES})
endif()

# Установка выходного файла
set_target_properties(graphics_engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Копирование ресурсов
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)