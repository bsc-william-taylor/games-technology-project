
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __GRU__TRANSFER__H__
#define __GRU__TRANSFER__H__

#include "EngineLayer.h"
#include "math.h"

// the number of layout attribute arrays available
#define SZ 6

// Some type defines to make the class a bit more readable !!!
typedef glm::vec3 Vertex;
typedef std::vector<Vertex> Vertices;
typedef std::vector<GLuint> Indices;
typedef GLuint GPU_ID;

// This class simply sends data to the GPU
class SILENCE_EXPORT GPU_Transfer
{
private:
	// The vertex array object ID
	GLuint vertexArrayObject;
	// The array of buffer IDs
	GLuint objectIDs[SZ];
	// The array of vectors of vertices
	Vertices * objectData[SZ];
	// A pointer to indices if any
	Indices * indices;
	// Has the data been sent
	GLboolean hasSent;
	//
	GLuint indexListSize;
public:
	// Constructor & Deconstructor
	GPU_Transfer();
	~GPU_Transfer();

	// sets the vertices colour data which will be sent
	GLvoid setVerticesColour(Vertices& colour);
	// sets the texture cords which will be sent
	GLvoid setTextureCords(Vertices& coords);
	// sets the vertices  which will be sent
	GLvoid setVertices(Vertices& vertices);
	// sets the normals which will be sent
	GLvoid setNormals(Vertices& normals);
	// sets the indices which will be sent
	GLvoid setIndices(Indices& indices);
	// sends all submitted data to the GPU
	GLvoid send();

	GLuint getIndexListSize();
	// returns the ID for the index buffer
	GLuint getIndexID();
	// returns the ID for the VAO
	GLuint getID();
};

#endif