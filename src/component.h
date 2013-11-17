#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

class GameObject;
class Mesh;
class Material;
class Shader;

class Component
{
public:
	virtual void Input(GameObject* pGameObject) {}
	virtual void Update(GameObject* pGameObject) {}
	virtual void Render(GameObject* pGameObject) {}

protected:
private:
};


class GameComponent : public Component {};

class RenderingComponent : public Component
{
    public:
	RenderingComponent(Mesh* pMesh = 0, Material* pMaterial = 0, Shader* pShader = 0);

	virtual void Render(GameObject* pGameObject);
	
	inline Mesh* GetMesh() {return m_pMesh;}
	inline Material* GetMaterial() {return m_pMaterial;}
	inline Shader* GetShader() {return m_pShader;}
protected:
private:
	Mesh* m_pMesh;
	Material* m_pMaterial;
	Shader* m_pShader;
};

class DefaultRenderingEngine : public RenderingComponent
{
public:
    virtual void Render(GameObject* pGameObject);
protected:
private:
};

#endif // COMPONENT_H_INCLUDED
