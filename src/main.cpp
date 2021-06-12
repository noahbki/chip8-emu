#include "Application.h"
#include "Logger.h"

int main(int argc, char** argv)
{
    Logger::the()->log("Starting...");
    Application::the()->create(640, 320, "CHIP-8 Emulator");
    Logger::the()->log("Exiting...");
    return 0;
}
