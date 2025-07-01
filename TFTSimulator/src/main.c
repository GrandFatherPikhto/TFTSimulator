#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

// Размеры "дисплея"
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define PIXEL_SCALE 4  // Увеличение для лучшей видимости

// Размеры шрифта
#define FONT_HEIGHT 12
#define FONT_WIDTH 7
#define BYTES_PER_ROW 1  // 7 пикселей умещаются в 1 байт


// Глобальные переменные SDL
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Буфер "дисплея" (1 бит на пиксель)
uint8_t display_buffer[DISPLAY_HEIGHT][DISPLAY_WIDTH / 8];

// Инициализация SDL
bool init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Эмулятор дисплея",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * PIXEL_SCALE, DISPLAY_HEIGHT * PIXEL_SCALE,
        SDL_WINDOW_SHOWN);

    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderSetScale(renderer, PIXEL_SCALE, PIXEL_SCALE);
    return true;
}

// Очистка буфера
void clear_buffer() {
    memset(display_buffer, 0, sizeof(display_buffer));
}

// Установка пикселя в буфере
void set_pixel(int x, int y, bool state) {
    if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT) {
        if (state) {
            display_buffer[y][x / 8] |= (1 << (7 - (x % 8)));
        } else {
            display_buffer[y][x / 8] &= ~(1 << (7 - (x % 8)));
        }
    }
}

// Отрисовка буфера в SDL
void render_buffer() {
    // Очищаем экран (черный фон)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Устанавливаем цвет "включенных" пикселей (белый)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Рисуем пиксели из буфера
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            // Проверяем бит в буфере
            if (display_buffer[y][x / 8] & (1 << (7 - (x % 8)))) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    // Обновляем экран
    SDL_RenderPresent(renderer);
}

// Получение данных символа
const uint8_t* get_char_data(char c) {
    // // Простая реализация для ASCII 0-9, a-z, A-Z
    // if (c >= '0' && c <= '9') {
    //     return &Font12_Table[(c - '0') * FONT_HEIGHT * BYTES_PER_ROW];
    // } else if (c >= 'A' && c <= 'Z') {
    //     return &Font12_Table[(10 + c - 'A') * FONT_HEIGHT * BYTES_PER_ROW];
    // } else if (c >= 'a' && c <= 'z') {
    //     return &Font12_Table[(36 + c - 'a') * FONT_HEIGHT * BYTES_PER_ROW];
    // }
    // // Возвращаем пробел для неизвестных символов
    // return &Font12_Table[0];
    return 0;
}

// Вывод символа в позицию (x,y)
void draw_char(int x, int y, char c) {
    const uint8_t* char_data = get_char_data(c);
    
    for (int row = 0; row < FONT_HEIGHT; row++) {
        uint8_t row_data = char_data[row * BYTES_PER_ROW];
        
        for (int col = 0; col < FONT_WIDTH; col++) {
            if (row_data & (1 << (7 - col))) {
                set_pixel(x + col, y + row, 1);
            }
        }
    }
}

// Вывод строки
void draw_string(int x, int y, const char* str) {
    while (*str) {
        draw_char(x, y, *str++);
        x += FONT_WIDTH + 1; // +1 пиксель между символами
    }
}

// Пример заполнения буфера (рисуем тестовый шаблон)
void draw_test_pattern() {
    // Горизонтальные линии
    for (int x = 0; x < DISPLAY_WIDTH; x++) {
        set_pixel(x, 0, true);
        set_pixel(x, DISPLAY_HEIGHT-1, true);
    }

    // Вертикальные линии
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        set_pixel(0, y, true);
        set_pixel(DISPLAY_WIDTH-1, y, true);
    }

    // Диагональ
    for (int i = 0; i < DISPLAY_WIDTH && i < DISPLAY_HEIGHT; i++) {
        set_pixel(i, i, true);
    }
}

// Главный цикл
int main() {
    if (!init_sdl()) {
        return 1;
    }

    clear_buffer();
    draw_test_pattern();

    bool running = true;
    while (running) {
        // Обработка событий
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // Отрисовка буфера
        render_buffer();

        // Небольшая задержка для снижения нагрузки на CPU
        SDL_Delay(16); // ~60 FPS
    }

    // Освобождение ресурсов
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}