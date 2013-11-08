#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <GL/glew.h>
#include "math3d.h"

typedef int INDEX;

struct Vertex
{
    Vector3f Pos;
    Vector2f TexCoord;
    
    Vertex(Vector3f pos = Vector3f(0,0,0), Vector2f texCoord = Vector2f(0,0), Vector3f normal = Vector3f(0,0,0))
	{
		Pos = pos;
		TexCoord = texCoord;
		//Normal = normal;
	}
};

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* vertices, int nVertices, INDEX* indices, int nIndices); 

	~Mesh();
	Mesh(const Mesh& mesh);
	void operator=(const Mesh& mesh);
	
	void Draw();
protected:
private:
	mutable bool	m_Cleanup;
	GLuint	        m_hVertexBuffer;
	GLuint	        m_hIndexBuffer;
	int				m_nVertices;
	int				m_nIndices;
};

#endif // MESH_H_INCLUDED
