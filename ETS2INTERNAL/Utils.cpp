#include "pch.h"
#include "Utils.h"

void writeToConsole(std::string text,bool error) {
    std::string output = "";

    if (error)
        output += "[Error] ";
    else
        output += "[LOG] ";

    output += text;
    std::cout << output << std::endl;
}