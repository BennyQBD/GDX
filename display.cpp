#include "display.h"
#include "engine.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

static SDL_Surface* g_pScreen = NULL;
static int g_Width;
static int g_Height;

bool Display::Init(int width, int height, const std::string& title)
{
    //TODO: Make this account for resizing!
    
    g_Width = width;
    g_Height = height;
    
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return false;
    }

    // atexit(Display::Destroy);
    
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    g_pScreen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    SetTitle(title);
    
    if ( !g_pScreen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return false;
    }
    
    GLenum glewError = glewInit(); 
    if(glewError != GLEW_OK) 
    { 
        printf("Error initializing GLEW: %s\n", glewGetErrorString(glewError)); 
        return false; 
    }
    
    //INIT OPENGL
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    
    //END INIT OPENGL
    
    return true;
}

void Display::Destroy()
{
    SDL_Quit();
}

void Display::SetTitle(const std::string& title)
{
    SDL_WM_SetCaption(title.c_str(), title.c_str());
}

void Display::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
}

void Display::ShowMouse(bool show)
{
    if(show)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);
}

void Display::SwapBuffers()
{
    SDL_GL_SwapBuffers();
}

void Display::ClearScreenAndDepth()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::ClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Display::ClearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Display::Error(const std::string& message)
{
    std::cout << "Error: " << message << std::endl;
    Engine::Stop();
}

void Display::SetMousePos(const Vector2f& pos)
{
    SDL_WarpMouse(pos.x, pos.y);
}

Vector2f Display::GetSize()
{
    return Vector2f(g_Width, g_Height);
}

Vector2f Display::GetCenter()
{
    return Vector2f(g_Width / 2, g_Height / 2);
}

