#include <stdio.h>
#include "display.h"
#include "engine.h"
#include "myGame.h"

#include "shader.h"
#include "renderer.h"

int main ( int argc, char** argv )
{
    Display::Init(800, 600, "Something awesome");

    MyGame myGame;
    
    Engine::GetGame()->SetGameComponent(&myGame);
    Engine::Start();
    
    Display::Destroy();
    printf("Exited cleanly\n");
    return 0;
}
