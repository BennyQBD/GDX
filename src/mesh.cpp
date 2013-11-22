#include "mesh.h"
#include "engine.h"

bool Vertex::FormatInitialized = false;
VertexFormat Vertex::Format = VertexFormat();

//TODO: Initialize primitives!
//Reference Rect, C++
//static Vertex g_pRectVertices[] = {Vertex(Vector3f(-0.5f,-0.5f,0),Vector2f(0,0)),
//                                   Vertex(Vector3f(-0.5f,0.5f,0), Vector2f(0,1)),
//                                   Vertex(Vector3f(0.5f,0.5f,0), Vector2f(1,1)),
//                                   Vertex(Vector3f(0.5f,-0.5f,0), Vector2f(1,0))};
//
//static int g_pRectIndices[] = {0,1,2,
//                               0,2,3};
//
//Mesh Mesh::Rect = Mesh(g_pRectVertices,sizeof(g_pRectVertices)/sizeof(Vertex),
//                       g_pRectIndices, sizeof(g_pRectIndices)/sizeof(int));

//Reference Plane, Java
//Vertex[] vertices = new Vertex[] {	new Vertex( new Vector3f(-0.5f, 0.0f, -0.5f), new Vector2f(0.0f, 0.0f)),
//											new Vertex( new Vector3f(-0.5f, 0.0f, 0.5f), new Vector2f(0.0f, 1.0f)),
//											new Vertex( new Vector3f(0.5f, 0.0f, -0.5f), new Vector2f(1.0f, 0.0f)),
//											new Vertex( new Vector3f(0.5f, 0.0f, 0.5f), new Vector2f(1.0f, 1.0f))};
//
//		int indices[] = new int[] {	0, 1, 2,
//									2, 1, 3};
//		
//		plane = new Mesh(vertices, indices, true);

//Reference Cube, Java
//Vertex[] vertices = new Vertex[] {	new Vertex(new Vector3f(-0.5f,-0.5f,-0.5f), new Vector2f(0,0)),
//											new Vertex(new Vector3f(-0.5f,0.5f,-0.5f), new Vector2f(0,1)),
//											new Vertex(new Vector3f(0.5f,0.5f,-0.5f), new Vector2f(1,1)),
//											new Vertex(new Vector3f(0.5f,-0.5f,-0.5f), new Vector2f(1,0)),
//						    				
//											new Vertex(new Vector3f(-0.5f,-0.5f,-0.5f), new Vector2f(0,0)),
//						    				new Vertex(new Vector3f(0.5f,-0.5f,-0.5f), new Vector2f(0,1)),
//						    				new Vertex(new Vector3f(0.5f,-0.5f,0.5f), new Vector2f(1,1)),
//						    				new Vertex(new Vector3f(-0.5f,-0.5f,0.5f), new Vector2f(1,0)),
//						    				
//						    				new Vertex(new Vector3f(-0.5f,-0.5f,-0.5f), new Vector2f(1,0)),
//						    				new Vertex(new Vector3f(-0.5f,-0.5f,0.5f), new Vector2f(0,0)),
//						    				new Vertex(new Vector3f(-0.5f,0.5f,0.5f), new Vector2f(0,1)),
//						    				new Vertex(new Vector3f(-0.5f,0.5f,-0.5f), new Vector2f(1,1)),
//						    				
//						    				new Vertex(new Vector3f(-0.5f,-0.5f,0.5f), new Vector2f(1,0)),
//						    				new Vertex(new Vector3f(0.5f,-0.5f,0.5f), new Vector2f(0,0)),
//						    				new Vertex(new Vector3f(0.5f,0.5f,0.5f), new Vector2f(0,1)),
//						    				new Vertex(new Vector3f(-0.5f,0.5f,0.5f), new Vector2f(1,1)),
//						    				
//						    				new Vertex(new Vector3f(-0.5f,0.5f,-0.5f), new Vector2f(1,0)),
//						    				new Vertex(new Vector3f(-0.5f,0.5f,0.5f), new Vector2f(0,0)),
//						    				new Vertex(new Vector3f(0.5f,0.5f,0.5f), new Vector2f(0,1)),
//						    				new Vertex(new Vector3f(0.5f,0.5f,-0.5f), new Vector2f(1,1)),
//						    				
//						    				new Vertex(new Vector3f(0.5f,-0.5f,-0.5f), new Vector2f(0,0)),
//						    				new Vertex(new Vector3f(0.5f,0.5f,-0.5f), new Vector2f(0,1)),
//						    				new Vertex(new Vector3f(0.5f,0.5f,0.5f), new Vector2f(1,1)),
//						    				new Vertex(new Vector3f(0.5f,-0.5f,0.5f), new Vector2f(1,0))};
//
//		int[] indices = new int[] {	0,1,2,
//				   					0,2,3,
//				   					
//				   					4,5,6,
//				   					4,6,7,
//				   					
//									8,9,10,
//									8,10,11,
//									
//									12,13,14,
//									12,14,15,
//									
//									16,17,18,
//									16,18,19,
//									
//									20,21,22,
//									20,22,23};
//		
//		cube = new Mesh(vertices, indices, true);

Mesh::Mesh()
{
	m_hVertexBuffer = 0;
	m_nVertices = 0;
	m_hIndexBuffer = 0;
	m_nIndices = 0;
	m_Cleanup = false;
	m_pVertexFormat = &Vertex::Format;
}

Mesh::Mesh(Vertex* vertices, int nVertices, INDEX* indices, int nIndices, bool calcNormals, bool calcTangents, const VertexFormat& vertexFormat)
{
    if(calcNormals)
        CalcNormals(vertices, nVertices, indices, nIndices);
    if(calcTangents)
        CalcTangents(vertices, nVertices, indices, nIndices);
    
	m_nVertices = nVertices;
	m_nIndices = nIndices;
	m_Cleanup = true;
    m_hVertexBuffer = Engine::GetRenderer()->CreateVertexBuffer(vertices, nVertices * sizeof(Vertex));
    m_hIndexBuffer =  Engine::GetRenderer()->CreateIndexBuffer(indices, nIndices * sizeof(int));
    m_pVertexFormat = &vertexFormat;
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
	m_nVertices = mesh.m_nVertices;
	m_hIndexBuffer = mesh.m_hIndexBuffer;
	m_nIndices = mesh.m_nIndices;
	m_Cleanup = mesh.m_Cleanup;
	m_pVertexFormat = mesh.m_pVertexFormat;
	mesh.m_Cleanup = false;
}

void Mesh::operator=(const Mesh& mesh)
{
	m_hVertexBuffer = mesh.m_hVertexBuffer;
	m_nVertices = mesh.m_nVertices;
	m_hIndexBuffer = mesh.m_hIndexBuffer;
	m_nIndices = mesh.m_nIndices;
	m_Cleanup = mesh.m_Cleanup;
	m_pVertexFormat = mesh.m_pVertexFormat;
	mesh.m_Cleanup = false;
}

void Mesh::Draw()
{
    Engine::GetRenderer()->DrawTriangles(m_hVertexBuffer, m_hIndexBuffer, m_nIndices, *m_pVertexFormat);
}

void Mesh::CalcNormals(Vertex* vertices, int nVertices, INDEX* indices, int nIndices)
{
    for(int i = 0; i < nIndices; i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];
			
        Vector3f v1 = vertices[i1].Pos - vertices[i0].Pos;
        Vector3f v2 = vertices[i2].Pos - vertices[i0].Pos;
        
        Vector3f normal = v1.Cross(v2).Normalized();
        
        vertices[i0].Normal += normal;
        vertices[i1].Normal += normal;
        vertices[i2].Normal += normal;
    }
		
    for(int i = 0; i < nVertices; i++)
        vertices[i].Normal = vertices[i].Normal.Normalized();
}
	
void Mesh::CalcTangents(Vertex* vertices, int nVertices, INDEX* indices, int nIndices)
{
    for(int i = 0; i < nIndices; i += 3)
    {
        Vertex v0 = vertices[indices[i]];
        Vertex v1 = vertices[indices[i + 1]];
        Vertex v2 = vertices[indices[i + 2]];
			
        Vector3f edge1 = v1.Pos - v0.Pos;
        Vector3f edge2 = v2.Pos - v0.Pos;
        
        float deltaU1 = v1.TexCoord.GetX() - v0.TexCoord.GetX();
        float deltaU2 = v2.TexCoord.GetX() - v0.TexCoord.GetX();
        float deltaV1 = v1.TexCoord.GetY() - v0.TexCoord.GetY();
        float deltaV2 = v2.TexCoord.GetY() - v0.TexCoord.GetY();
        
        float f = 1.0f/(deltaU1 * deltaV2 - deltaU2 * deltaV1);
        
        Vector3f tangent = Vector3f(0,0,0);
        
        tangent.SetX(f * (deltaV2 * edge1.GetX() - deltaV1 * edge2.GetX()));
        tangent.SetY(f * (deltaV2 * edge1.GetY() - deltaV1 * edge2.GetY()));
        tangent.SetZ(f * (deltaV2 * edge1.GetZ() - deltaV1 * edge2.GetZ()));

//Bitangent example, in Java
//		Vector3f bitangent = new Vector3f(0,0,0);
//		
//		bitangent.setX(f * (-deltaU2 * edge1.getX() - deltaU1 * edge2.getX()));
//		bitangent.setX(f * (-deltaU2 * edge1.getY() - deltaU1 * edge2.getY()));
//		bitangent.setX(f * (-deltaU2 * edge1.getZ() - deltaU1 * edge2.getZ()));
		
		v0.Tangent += tangent;
		v1.Tangent += tangent;
		v2.Tangent += tangent;	
    }
		
    for(int i = 0; i < nVertices; i++)
        vertices[i].Tangent = vertices[i].Tangent.Normalized();
}
