#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

class GameObject;

class Component
{
public:
	virtual void Input(GameObject* pGameObject) {}
	virtual void Update(GameObject* pGameObject) {}
	virtual void Render(GameObject* pGameObject) {}

protected:
private:
};

class RenderingComponent : public Component {};
class GameComponent : public Component {};

class DefaultRenderingEngine : public RenderingComponent
{
public:
    virtual void Render(GameObject* pGameObject);
protected:
private:
};

#endif // COMPONENT_H_INCLUDED
