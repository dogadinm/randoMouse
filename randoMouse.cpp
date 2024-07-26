// randoMouse.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <random>
#include <chrono>
#include <thread>
#include <time.h>

// дополнительный поток что бы убить оригинальный 
DWORD WINAPI DopPotok(LPVOID lpParam) {
    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE)) { // Проверяем, была ли нажата клавиша Esc
            exit(0); // Завершаем программу при нажатии Esc
        }
        Sleep(100); // Даем немного времени перед следующей проверкой (100 миллисекунд)
    }
}

// Функция для генерации случайного числа в заданном диапазоне
int getRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Функция для плавного перемещения мыши из одной точки в другую
void smoothMove(int startX, int startY, int endX, int endY, int steps, int delay) {
    int deltaX = endX - startX;
    int deltaY = endY - startY;
    for (int i = 0; i <= steps; ++i) {
        int x = startX + (deltaX * i / steps);
        int y = startY + (deltaY * i / steps);
        SetCursorPos(x, y);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

int main() {

    // Размеры экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    bool flag = true;

    HANDLE hThread = CreateThread(NULL, 0, DopPotok, NULL, 0, NULL);

    if (hThread == NULL) {
        std::cerr << "Ошибка при создании потока: " << GetLastError() << std::endl;
        return 1;
    }

    while (flag) {
        // Получение текущих координат курсора
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        int startX = cursorPos.x;
        int startY = cursorPos.y;
        

        // Генерация случайных координат в пределах экрана
        int endX = getRandomNumber(0, screenWidth);
        int endY = getRandomNumber(0, screenHeight);

        std::cout << endX << " " << endY << std::endl;

        // Плавное перемещение мыши на новые координаты
        smoothMove(startX, startY, endX, endY, 100, 10);

        // Случайная пауза от 100 до 1000 миллисекунд
        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(100, 1000)));
    }

    return 0;
}