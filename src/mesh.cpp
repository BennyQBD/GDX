#include "mesh.h"
#include "engine.h"

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
    m_hVertexBuffer = Engine::GetRenderer()->CreateVertexBuffer(vertices, nVertices * sizeof(Vertex));
    m_hIndexBuffer =  Engine::GetRenderer()->CreateIndexBuffer(indices, nIndices * sizeof(int));
}

Mesh::~Mesh()
{
	if(m_Cleanup)
	{
	    if(m_hVertexBuffer)	Engine::GetRenderer()->DeleteBuffer(m_hVertexBuffer);
		if(m_hIndexBuffer)	Engine::GetRenderer()->DeleteBuffer(m_hIndexBuffer);
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
    Engine::GetRenderer()->DrawTriangles(m_hVertexBuffer, m_hIndexBuffer, m_nIndices);
}
