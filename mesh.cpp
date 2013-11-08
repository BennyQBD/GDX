#include "mesh.h"

Mesh::Mesh()
{
	m_hVertexBuffer = 0;
	m_nVertices = 0;
	m_hIndexBuffer = 0;
	m_nIndices = 0;
	m_Cleanup = false;
}

Mesh::Mesh(Vertex* vertices, int nVertices, INDEX* indices, int nIndices)
{
	m_nVertices = nVertices;
	m_nIndices = nIndices;
	m_Cleanup = true;

	glGenBuffers(1, &m_hVertexBuffer);
    glGenBuffers(1, &m_hIndexBuffer);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(int), indices, GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
	if(m_Cleanup)
	{
		if(m_hVertexBuffer)	glDeleteBuffers(1, &m_hVertexBuffer);
		if(m_hIndexBuffer)	glDeleteBuffers(1, &m_hIndexBuffer);
	}
}

//TODO: This needs to be a DEEP COPY!
Mesh::Mesh(const Mesh& mesh)
{
	m_hVertexBuffer = mesh.m_hVertexBuffer;
	m_nVertices =mesh.m_nVertices;
	m_hIndexBuffer = mesh.m_hIndexBuffer;
	m_nIndices = mesh.m_nIndices;
	m_Cleanup = mesh.m_Cleanup;
	mesh.m_Cleanup = false;
}

void Mesh::operator=(const Mesh& mesh)
{
	m_hVertexBuffer = mesh.m_hVertexBuffer;
	m_nVertices =mesh.m_nVertices;
	m_hIndexBuffer = mesh.m_hIndexBuffer;
	m_nIndices = mesh.m_nIndices;
	m_Cleanup = mesh.m_Cleanup;
	mesh.m_Cleanup = false;
}

void Mesh::Draw()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3f));
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f) + sizeof(Vector2f)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIndexBuffer);
    glDrawElements(GL_TRIANGLES, m_nIndices, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(2);
}
