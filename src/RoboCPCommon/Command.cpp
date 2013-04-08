#pragma once
#include <Command.h>

Command::Command ()
{
  sec = 0;
  angle = 0;
}

Command::Command (int s, float a)
{
  sec = s;
  angle = a;
}