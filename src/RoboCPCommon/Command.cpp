#pragma once
#include <Command.h>

Command::Command()
{
  ComType = 0;
  ComCondition = 0;
  Value = 0;
  Time = time(NULL);
}

Command::Command (CommandType CType, CommandCondition CCondition, float ConditionValue)
{
  ComType = CType;
  ComCondition = CCondition;
  Value = ConditionValue;
  Time = time(NULL);
}