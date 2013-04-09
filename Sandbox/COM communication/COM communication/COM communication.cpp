// COM communication.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "SerialCom.h"
#include <Windows.h>
#include <WinDef.h>
#include <stdio.h>

SerialCom *C;
char *Ch;

int _tmain(int argc, _TCHAR* argv[])
{
  C = new SerialCom(L"COM3");
  while (true){
    Ch = C->Read();
    int i = 0;
    while (Ch[i]!='\0'){
      printf("%c",Ch[i]);
      i++;
    }
  }
  C->~SerialCom();
	return 0;
}

