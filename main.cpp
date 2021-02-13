#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <string>
#include <chrono>


#ifdef __MSC_VER
#include <Windows.h>
#endif


void SetCursorPos(int x, int y) {
#ifdef __MSC_VER
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

void Resize(int x, int y, int width, int height) {
#ifdef __MSC_VER
    SMALL_RECT windowSize = { x , y , x + width , y + height };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize)
#else
    printf("\e[8;%d;%dt", height, width);
#endif
}


using mat = std::vector<std::vector<char>>;

void clear(mat& m, int width, int height, char c = ' ') {
    m.resize(height);
    for (int y = 0; y < height; ++y) {
        m[y].resize(width);
        for (int x = 0; x < width; ++x) {
            m[y][x] = c;
        }
    }
}

void draw(mat m1, mat& m2, int width, int height) {
    Resize(0, 0, width, height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (m1[y][x] != m2[y][x]) {
                SetCursorPos(x, y);
                std::cout << m2[y][x] << std::flush;
            }
        }
    }
    std::cout << std::flush;
    std::swap(m1, m2);
}


int main() {
    const std::vector<std::string> text = { "Sanya hui sosi", "Nahui ti rodilsya blyat", "", "", "", "", "P.S.: Ya ne shuchu" };

    constexpr size_t width = 64;
    constexpr size_t height = 32;
    constexpr double scaleY = 2.5;

    mat m1;
    mat m2;

    clear(m1, width, height, '\0');
    clear(m2, width, height);

    double t = 0;
    double textOffsetD = text.size();

    auto last = std::chrono::high_resolution_clock::now();
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        double delta = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count() / 1000000000.0;
        last = now;

        clear(m2, width, height);

        float f = (std::sin(t) + 1) * 0.25f + 0.5f;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                double yy = (y - height / 2.0) * scaleY;
                double xx = x - width / 2.0;

                double xxx = -xx / 16.0;
                double yyy = (-16 - yy) / 16.0;

                xxx /= f;
                yyy /= f;

                if (yyy < std::sqrt(1 - std::pow(std::abs(xxx) - 1, 2)) && yyy > std::acos(1 - std::abs(xxx)) - 3.1415) {
                    m2[y][x] = '#';
                }

                size_t textOffset = (y + (int)textOffsetD) % ((int)height + text.size());
                if (text.size() > textOffset) {
                    const std::string& t = text[textOffset];
                    int p = x - ((int)(width / 2) - t.size() / 2);
                    if (p >= 0 && p < t.size()) {
                        m2[y][x] = t[p];
                    }
                }
            }
        }

        draw(m1, m2, width, height);

        t += delta * 3.5;

        textOffsetD += delta * 2;
    }

    return 0;
}
