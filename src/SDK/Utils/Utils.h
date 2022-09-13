#pragma once
#include "../../pch.h"

template <typename T>
void LogError(T msg)
{
    std::cout << "[Error] " << msg << std::endl;
}

template <typename T>
void LogInfo(T msg)
{
    std::cout << "[Info] " << msg << std::endl;
}

void AttachConsole();

void CloseConsole();
