#include "gameObject.h"
#include <algorithm>

GameObject::GameObject(const Transform& transform, RenderingComponent* pRenderingComponent)
{
	m_Transform = transform;
	m_pRenderingComponent = pRenderingComponent;
	m_ChildObjects = std::vector<GameObject*>();
}

void GameObject::Input()
{
	m_pRenderingComponent->Input(this);

	for(std::vector<GameObject*>::iterator it = m_ChildObjects.begin(); it != m_ChildObjects.end(); ++it)
		(*it)->Input();
}

void GameObject::Update()
{
	m_pRenderingComponent->Update(this);

	for(std::vector<GameObject*>::iterator it = m_ChildObjects.begin(); it != m_ChildObjects.end(); ++it)
		(*it)->Update();
}

void GameObject::Render()
{
	m_pRenderingComponent->Render(this);

	for(std::vector<GameObject*>::iterator it = m_ChildObjects.begin(); it != m_ChildObjects.end(); ++it)
	{
		(*it)->GetTransform().SetChildModel(m_Transform.GetModel(false));
		(*it)->Render();
	}
}

void GameObject::AddChild(GameObject* gameObject)
{
	m_ChildObjects.push_back(gameObject);
}

void GameObject::RemoveChild(GameObject* gameObject)
{
	m_ChildObjects.erase(std::remove(m_ChildObjects.begin(), m_ChildObjects.end(), gameObject), m_ChildObjects.end());
}

void GameObject::RemoveChild(int index)
{
	m_ChildObjects.erase(m_ChildObjects.begin() + index);
}

Transform& GameObject::GetTransform()
{
	return m_Transform;
}

RenderingComponent* GameObject::GetRenderingComponent() const
{
	return m_pRenderingComponent;
}

void GameObject::SetTransform(const Transform& transform)
{
	m_Transform = transform;
}
