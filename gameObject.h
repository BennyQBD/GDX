#ifndef GAMEOBJECT_H
#define GAME_OBJECT_H

#include "transform.h"
#include "renderingComponent.h"
#include <vector>

class GameObject
{
public:
	GameObject(const Transform& transform = Transform(), RenderingComponent* pRenderingComponent = 0);

	void Input();
	void Update();
	void Render();

	void AddChild(GameObject* gameObject);
	void RemoveChild(GameObject* gameObject);
	void RemoveChild(int index);

	Transform& GetTransform();
	RenderingComponent* GetRenderingComponent() const;

	void SetTransform(const Transform& transform);
protected:
private:
	std::vector<GameObject*> m_ChildObjects;
	Transform m_Transform;
	RenderingComponent* m_pRenderingComponent;
};

#endif
