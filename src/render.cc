#include <memory>

#include "Controller.h"

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

int main(int argc, char** argv)
{
    std::shared_ptr<App::Controller> controller = std::make_shared<App::Controller>(SCR_WIDTH, SCR_HEIGHT);
    controller->Run();
    return 0;
}



