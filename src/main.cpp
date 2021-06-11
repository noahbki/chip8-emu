#include "Logger.h"


int main(int argc, char** argv)
{
    Logger::the()->log("Starting...");
    Logger::the()->log("Exiting...");
    return 0;
}
