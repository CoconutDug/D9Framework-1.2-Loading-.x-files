#pragma once
#include "Types.h"
#include <Windows.h>
#include <Winbase.h>


void Error(const char* message);

#define ErrorMessage(message) Error(message);
#define AssertMessage(condition,message) if(!(condition)) Error(message);
