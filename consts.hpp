#ifndef CONST_HPP
#define CONST_HPP
#include <string>

std::string const FOREGROUND_RED = "\x1b[31m";
std::string const FOREGROUND_GREEN = "\x1b[32m";
std::string const FOREGROUND_YELLOW = "\x1b[33m";
std::string const FOREGROUND_BLUE = "\x1b[34m";
std::string const FOREGROUND_MAGENTA = "\x1b[35m";
std::string const FOREGROUND_CYAN = "\x1b[36m";
std::string const FOREGROUND_WHITE = "\x1b[37m";
std::string const BACKGROUND_RED = "\x1b[41m";
std::string const BACKGROUND_GREEN = "\x1b[42m";
std::string const BACKGROUND_YELLOW = "\x1b[43m";
std::string const BACKGROUND_BLUE = "\x1b[44m";
std::string const BACKGROUND_MAGENTA = "\x1b[45m";
std::string const BACKGROUND_CYAN = "\x1b[46m";
std::string const BACKGROUND_WHITE = "\x1b[47m";
std::string const RESET_COLOR = "\x1b[0m";
std::string const CLEAR_TERMINAL = "\033[2J\033[1;1H";

#endif