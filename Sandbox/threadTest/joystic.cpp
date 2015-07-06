#include <iostream>
#include <QtCore\qthread.h>
#include <SDL.h>

#include <QtCore/QDebug>
 
QT_USE_NAMESPACE

int joystick()
{
    // Переменная для нашего джойстика
    SDL_Joystick *joy;   
    SDL_Event event;
 
    // Инициализация SDL для использования джойстика
    SDL_Init(SDL_INIT_JOYSTICK);
    // Включаем
    SDL_JoystickEventState(SDL_ENABLE);
    // Открываем ;)
    joy = SDL_JoystickOpen(0);
 
    while(1)
    {
        // Примитивнейшая задержка
        unsigned int j = 0;
        for (j = 0; j < 60000; j++);
 
        SDL_PollEvent(&event);
 
        // Получаем значения, соответствующие смещению джойстика
        // по оси Х
        int xAxis = SDL_JoystickGetAxis(joy, 0);
        // по оси Y
        int yAxis = SDL_JoystickGetAxis(joy, 1);
 
        // Выводим
        qDebug() << xAxis << yAxis;
    }
}