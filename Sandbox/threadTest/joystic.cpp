#include <iostream>
#include <QtCore\qthread.h>
#include <SDL.h>

#include <QtCore/QDebug>
 
QT_USE_NAMESPACE

int joystick()
{
    // ���������� ��� ������ ���������
    SDL_Joystick *joy;   
    SDL_Event event;
 
    // ������������� SDL ��� ������������� ���������
    SDL_Init(SDL_INIT_JOYSTICK);
    // ��������
    SDL_JoystickEventState(SDL_ENABLE);
    // ��������� ;)
    joy = SDL_JoystickOpen(0);
 
    while(1)
    {
        // �������������� ��������
        unsigned int j = 0;
        for (j = 0; j < 60000; j++);
 
        SDL_PollEvent(&event);
 
        // �������� ��������, ��������������� �������� ���������
        // �� ��� �
        int xAxis = SDL_JoystickGetAxis(joy, 0);
        // �� ��� Y
        int yAxis = SDL_JoystickGetAxis(joy, 1);
 
        // �������
        qDebug() << xAxis << yAxis;
    }
}