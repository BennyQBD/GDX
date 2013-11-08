#ifndef RENDERINGCOMPONENT_H
#define RENDERINGCOMPONENT_H

class GameObject;

class RenderingComponent
{
public:
	virtual void Input(GameObject* pGameObject) {}
	virtual void Update(GameObject* pGameObject) {}
	virtual void Render(GameObject* pGameObject) {}

protected:
private:
};

#endif
