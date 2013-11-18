#include "myGame.h"
#include "input.h"
#include "engine.h"
#include "transform.h"
#include "mesh.h"
#include "shader.h"
#include "timing.h"
#include "material.h"
#include "input.h"
#include "camera.h"
#include "display.h"
#include "gameObject.h"
#include "texture.h"

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
static PerspectiveCamera	g_Camera;
static GameObject			g_GameObject1;
static GameObject			g_GameObject2;
static RenderingComponent	g_MeshRenderer;

static Texture             g_Texture;
static Material			g_Material;
static Mesh				g_Mesh;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Class methods
//--------------------------------------------------------------------------------------
MyGame::MyGame()
{
	Vertex vertices[] = {Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
						 Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
						 Vertex(Vector3f(1.0f, -1.0f, 0.5773f), Vector2f(1.0f, 0.0f)),
						 Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.5f, 1.0f))};

	INDEX indices[] = { 0, 3, 1,
						1, 3, 2,
						2, 3, 0,
						1, 2, 0};

	g_Mesh = Mesh(vertices, sizeof(vertices)/sizeof(Vertex), indices, sizeof(indices)/sizeof(INDEX));

    g_Texture = Texture("bricks.jpg");
    g_Material = Material(g_Texture);

	g_MeshRenderer = RenderingComponent(&g_Mesh, &g_Material, Shader::Get("basicShader"));

	g_GameObject1 = GameObject(Transform(), &g_MeshRenderer);
	g_GameObject2 = GameObject(Transform(Vector3f(), Quaternion(), Vector3f(0.3f, 0.3f, 0.3f)), &g_MeshRenderer);
	g_Camera = PerspectiveCamera(Vector3f(0, 1, -5), Vector3f::FORWARD, Vector3f::UP, ToRadians(70.0f), 0.01f, 1000.0f);
}

MyGame::~MyGame()
{

}

void MyGame::Input(GameObject* pGameObject)
{    
	static bool mouseLocked = false;

	const float ROTSPEEDX = 150.0f;
	const float ROTSPEEDY = 175.0f;

	const float MOVSPEED = 10.0f;
	const float STRAFESPEED = MOVSPEED;
	const float SENSITIVITY_X = 0.004f;	
	const float SENSITIVITY_Y = SENSITIVITY_X * 1.0f;

	double delta = Time::GetDelta();

	if(Input::GetKey(SDLK_UP))
		g_Camera.Pitch(ToRadians(ROTSPEEDX * delta));
	if(Input::GetKey(SDLK_DOWN))
		g_Camera.Pitch(ToRadians(-ROTSPEEDX * delta));
	if(Input::GetKey(SDLK_LEFT))
		g_Camera.Yaw(ToRadians(ROTSPEEDY * delta));
	if(Input::GetKey(SDLK_RIGHT))
		g_Camera.Yaw(ToRadians(-ROTSPEEDY * delta));

	if(Input::GetKey(SDLK_q))
		g_Camera.Roll(ToRadians(-ROTSPEEDX * delta));
	if(Input::GetKey(SDLK_e))
		g_Camera.Roll(ToRadians(ROTSPEEDX * delta));

	if(Input::GetKey(SDLK_w))
		g_Camera.Move(g_Camera.GetForward() * (float)(MOVSPEED * delta));
	if(Input::GetKey(SDLK_s))
		g_Camera.Move(g_Camera.GetBack() * (float)(MOVSPEED * delta));
	if(Input::GetKey(SDLK_a))
		g_Camera.Move(g_Camera.GetLeft() * (float)(STRAFESPEED * delta));
	if(Input::GetKey(SDLK_d))
		g_Camera.Move(g_Camera.GetRight() * (float)(STRAFESPEED * delta));

	Vector2f centerPos = Display::GetCenter();

	if(mouseLocked)
	{
		Vector2f deltaPos = centerPos - Input::GetMousePos();

		bool rotX = (int)(deltaPos.GetY()) != 0;
		bool rotY = (int)(deltaPos.GetX()) != 0;

		if(rotY)
			g_Camera.RotateY(deltaPos.GetX() * SENSITIVITY_X);
		if(rotX)
			g_Camera.Pitch(deltaPos.GetY() * SENSITIVITY_Y);

		if(rotX || rotY)
			Display::SetMousePos(centerPos);
	}

	if(Input::GetMouseDown(SDL_BUTTON_LEFT))
	{
		mouseLocked = true;
		Display::ShowMouse(false);
		Display::SetMousePos(centerPos);
	}

	if(Input::GetKeyDown(SDLK_ESCAPE))
	{
		mouseLocked = false;
		Display::ShowMouse(true);
	}
}

void MyGame::Update(GameObject* pGameObject)
{
	g_GameObject1.GetTransform().Rotation = Quaternion(Vector3f::UP, (float)Time::GetElapsedTime());

	g_GameObject2.GetTransform().Pos = Vector3f(-4.0f, 0.0f, 0.0f).Rotate(Quaternion(Vector3f::UP, (float)Time::GetElapsedTime() * 2.0f));
	g_GameObject2.GetTransform().Rotation = Quaternion(Vector3f::FORWARD, -(float)Time::GetElapsedTime());
}

void MyGame::Render(GameObject* pGameObject)
{
    Transform::CalcViewProjection(g_Camera);

	g_GameObject1.Render();
	g_GameObject2.Render();
}
