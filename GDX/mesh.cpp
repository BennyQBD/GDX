#include "mesh.h"
#include "engine.h"
#include "display.h"

#include <fstream>
#include <sstream>

bool Vertex::FormatInitialized = false;
VertexFormat Vertex::Format = VertexFormat();

std::map<std::string, Mesh*> Mesh::Meshes = std::map<std::string, Mesh*>();

Mesh* Mesh::Get(const std::string& name)
{
	std::map<std::string, Mesh*>::iterator it = Meshes.find(name);
	if(it != Meshes.end())
		return it->second;

	Meshes.insert(std::pair<std::string, Mesh*>(name, new Mesh(name)));
    return Meshes.at(name);
}

Mesh* Mesh::Create(const std::string& name, Vertex* vertices, int nVertices, INDEX* indices, int nIndices, bool calcNormals, bool calcTangents, const VertexFormat& vertexFormat)
{
	if(Meshes.find(name) != Meshes.end())
		Display::Error("Mesh " + name + " already exists, and therefore cannot be created.");

	Meshes.insert(std::pair<std::string, Mesh*>(name, new Mesh(vertices, nVertices, indices, nIndices, calcNormals, calcTangents, vertexFormat)));
    return Meshes.at(name);
}

void Mesh::Delete(const std::string& name)
{
	std::map<std::string, Mesh*>::iterator it = Meshes.find(name);
	if(it != Meshes.end()) 
	{
		delete it->second;
		Meshes.erase(it);
	} 
}

void Mesh::DeleteAll()
{
    for (std::map<std::string, Mesh*>::iterator it=Meshes.begin(); it!=Meshes.end(); ++it)
        delete it->second;
}

Mesh::Mesh(Vertex* vertices, int nVertices, INDEX* indices, int nIndices, bool calcNormals, bool calcTangents, const VertexFormat& vertexFormat)
{
    if(calcNormals)
        CalcNormals(vertices, nVertices, indices, nIndices);
    if(calcTangents)
        CalcTangents(vertices, nVertices, indices, nIndices);
    
	m_nVertices = nVertices;
	m_nIndices = nIndices;
    m_hVertexBuffer = Engine::GetRenderer()->CreateVertexBuffer(vertices, nVertices * sizeof(Vertex));
    m_hIndexBuffer =  Engine::GetRenderer()->CreateIndexBuffer(indices, nIndices * sizeof(int));
    m_pVertexFormat = &vertexFormat;
}

std::vector<std::string> split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
        elems.push_back(item);

    return elems;
    return elems;
}

Mesh::Mesh(const std::string& fileName)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3f> vertices;
	std::vector<Vector2f> uvs;
	std::vector<Vector3f> normals;

	std::ifstream file;
    file.open(("./res/models/" + fileName).c_str());

    std::string line;

	bool hasUVs = false;
	bool hasNormals = false;

    if(file.is_open())
    {
        while(file.good())
        {
			getline(file, line);
			std::vector<std::string> tokens = split(line, ' ');

			if(tokens.size() == 0)
				continue;

			if(tokens[0].compare("v") == 0)
				vertices.push_back(Vector3f(std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3])));
			else if(tokens[0].compare("vt") == 0)
				uvs.push_back(Vector2f(std::stof(tokens[1]),std::stof(tokens[2])));
			else if(tokens[0].compare("vn") == 0)
				normals.push_back(Vector3f(std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3])));
			else if(tokens[0].compare("f") == 0)
			{
				for(int i = 1; i < (int)tokens.size(); i++)
				{
					std::vector<std::string> vert = split(tokens[i], '/');

					vertexIndices.push_back(std::stoi(vert[0]) - 1);

					if(vert.size() > 1 && vert[1].compare("") != 0)
					{
						uvIndices.push_back(std::stoi(vert[1]) - 1);
						hasUVs = true;
					}
					else if(vert.size() > 2 && vert[2].compare("") != 0)
					{
						normalIndices.push_back(std::stoi(vert[2]) - 1);
						hasNormals = true;
					}
				}
			}
        }
    }
    else
    {
        Display::Error("Unable to load shader: " + fileName);
    }

	std::vector<Vertex> vertexList;
	std::vector<INDEX> indexList;
	
	std::map<int, int> indexMap;

	for(int i = 0; i < (int)vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = 0; 
		unsigned int normalIndex = 0;

		if(hasUVs)
			uvIndex = uvIndices[i];
		if(hasNormals)
			normalIndex = normalIndices[i];

		int previousVertexLocation = -1;

		for(int j = 0; j < i; j++)
		{
			if(vertexIndices[j] == vertexIndex
				&& (!hasUVs || uvIndices[j] == uvIndex)
				&& (!hasNormals || normalIndices[j] == normalIndex))
			{
				previousVertexLocation = j;
				break;
			}
		}

		if(previousVertexLocation == -1)
		{
			indexMap.insert(std::pair<int, int>(i, vertexList.size()));
			indexList.push_back(vertexList.size());

			Vector3f pos = vertices[vertexIndex];
			Vector2f texCoord;
			Vector3f normal;

			if(hasUVs)
				texCoord = uvs[uvIndex];
			else
				texCoord = Vector2f(0,0);

			if(hasNormals)
				normal = normals[normalIndex];
			else
				normal = Vector3f(0,0,0);

			vertexList.push_back(Vertex(pos,texCoord,normal));
		}
		else
			indexList.push_back(indexMap.at(previousVertexLocation));
	}

	if(!hasUVs)
		CalcTexCoords(&vertexList[0],vertexList.size(),&indexList[0],indexList.size());
	if(!hasNormals)
		CalcNormals(&vertexList[0],vertexList.size(),&indexList[0],indexList.size());

	CalcTangents(&vertexList[0],vertexList.size(),&indexList[0],indexList.size());

	m_nVertices = vertexList.size();
	m_nIndices = indexList.size();
    m_hVertexBuffer = Engine::GetRenderer()->CreateVertexBuffer(&vertexList[0], m_nVertices * sizeof(Vertex));
    m_hIndexBuffer =  Engine::GetRenderer()->CreateIndexBuffer(&indexList[0], m_nIndices * sizeof(int));
	m_pVertexFormat = &Vertex::Format;
}

Mesh::~Mesh()
{
	if(m_hVertexBuffer)	Engine::GetRenderer()->DeleteBuffer(m_hVertexBuffer);
	if(m_hIndexBuffer)	Engine::GetRenderer()->DeleteBuffer(m_hIndexBuffer);
}

void Mesh::Draw()
{
    Engine::GetRenderer()->DrawTriangles(m_hVertexBuffer, m_hIndexBuffer, m_nIndices, *m_pVertexFormat);
}

void Mesh::CalcTexCoords(Vertex* vertices, int nVertices, INDEX* indices, int nIndices)
{
	//static Vector2f texCoords[] = {Vector2f(0.0f, 0.0f),Vector2f(0.0f, 1.0f),Vector2f(1.0f, 1.0f), Vector2f(1.0f, 0.0f)};
	static Vector2f texCoords[] = {Vector2f(0.0f, 0.0f),Vector2f(0.5f, 1.0f),Vector2f(1.0f, 0.0f)};

	for(int i = 0; i < nIndices; i++)
	{
		int i0 = indices[i];

		Vector2f texCoord = Vector2f(texCoords[i0 % (sizeof(texCoords)/sizeof(Vector2f))]);

		//texCoord = (texCoord * ((float)i/(float)nIndices));

		//float value = 1.0f - 2 *(float)i0/(float)nIndices;

		//Vector2f texCoord = Vector2f(value, value);

		vertices[i0].TexCoord = texCoord;
	}

	/*for(int i = 0; i < nVertices; i++)
		vertices[i].TexCoord = vertices[i].TexCoord.Normalized();*/
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
