#include <stdio.h>
#include "GDX.h"

int main(int argc, char** argv)
{
	Engine::GetDisplay()->Init(800, 600, "Something awesome");

	GameObject box = GameObject(Transform(), RenderingComponent::Create("shadedEgg", Mesh::Get("egg1.obj"), Material::Get("basicBrick.xmt"), Shader::Get("phongShader")));

	Engine::GetGame()->AddGameObject(&box);
    Engine::Start();
    
	Engine::GetDisplay()->Destroy();
    printf("Exited cleanly\n");
    return 0;
}
