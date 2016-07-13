
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

#include "GPU_Transfer.h"

// Macros to help with buffer IDs
#define TEXTURE_ID 2
#define VERTEX_ID 0
#define NORMAL_ID 1
#define COLOUR_ID 3
#define INDICE_ID 4
#define MODEL_ID 4

// Constructor & Deconstructor
GPU_Transfer::GPU_Transfer()
	: indices(NULL),
	  hasSent(GL_FALSE)
{
	glGenVertexArrays(1, &vertexArrayObject);
	indexListSize = NULL;
	for (int i = 0; i < SZ; i++)  
	{
		glGenBuffers(1, &objectIDs[i]);
		objectData[i] = NULL;
	}
}

GPU_Transfer::~GPU_Transfer()
{
	// here we cleanup any buffers to be deleted
	glDeleteVertexArrays(1, &vertexArrayObject);
	for (int i = 0; i < SZ; i++)
		objectIDs[i] ? glDeleteBuffers(1, &objectIDs[i]) : NULL;
}

// returns the ID for the VAO
GLuint GPU_Transfer::getID()
{
	if (hasSent)
	{
		return vertexArrayObject;
	}
	else
	{
		return NULL;
	}
}

GLuint GPU_Transfer::getIndexListSize()
{
	return indexListSize;
}

// returns the INDICES ID
GLuint GPU_Transfer::getIndexID()
{
	return objectIDs[INDICE_ID];
}

// Sends submitted data to the gpu
void GPU_Transfer::send() 
{
	// Bind the VAO which will manage our multiple buffer objects
	glBindVertexArray(vertexArrayObject);
	hasSent = GL_TRUE;
	
	// if vertex data has been set
	if (objectData[VERTEX_ID] != NULL) 
	{
		// then bind it for use
		glBindBuffer(GL_ARRAY_BUFFER, objectIDs[VERTEX_ID]);
		// submit data to it
		glBufferData(GL_ARRAY_BUFFER,
			(*objectData[VERTEX_ID]).size() * (sizeof(GLfloat) * 3),
			(*objectData[VERTEX_ID]).data(),
			GL_STATIC_DRAW
		);

		// and then bind it to a layout location ID
		glEnableVertexAttribArray(VERTEX_ID);
		glVertexAttribPointer(VERTEX_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		objectData[VERTEX_ID] = NULL;
	}

	// if normal data has been set
	if (objectData[NORMAL_ID] != NULL) 
	{
		// then bind it for use
		glBindBuffer(GL_ARRAY_BUFFER, objectIDs[NORMAL_ID]);
		// submit data to it
		glBufferData(GL_ARRAY_BUFFER, 
			(*objectData[NORMAL_ID]).size() * (sizeof(GLfloat) * 3),
			(*objectData[NORMAL_ID]).data(), 
			GL_STATIC_DRAW
		);

		// and then bind it to a layout location ID
		glEnableVertexAttribArray(NORMAL_ID);
		glVertexAttribPointer(NORMAL_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		objectData[NORMAL_ID] = NULL;
	}

	// if texture data has been set
	if (objectData[TEXTURE_ID] != NULL) 
	{
		// then bind it for use
		glBindBuffer(GL_ARRAY_BUFFER, objectIDs[TEXTURE_ID]);
		// submit data to it
		glBufferData(GL_ARRAY_BUFFER,
			(*objectData[TEXTURE_ID]).size() * (sizeof(GLfloat) * 3),
			(*objectData[TEXTURE_ID]).data(),
			GL_STATIC_DRAW
		);

		// and then bind it to a layout location ID
		glEnableVertexAttribArray(TEXTURE_ID);
		glVertexAttribPointer(TEXTURE_ID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		objectData[TEXTURE_ID] = NULL;
	}

	// if colour data has been set
	if (objectData[COLOUR_ID] != NULL) 
	{
		// then bind it for use
		glBindBuffer(GL_ARRAY_BUFFER, objectIDs[COLOUR_ID]);
		// submit data to it
		glBufferData(GL_ARRAY_BUFFER, 
			(*objectData[COLOUR_ID]).size() * (sizeof(GLfloat)*3), 
			(*objectData[COLOUR_ID]).data(),
			GL_STATIC_DRAW
		);

		// and then bind it to a layout location ID
		glEnableVertexAttribArray(COLOUR_ID);
		glVertexAttribPointer(COLOUR_ID, 4, GL_FLOAT, GL_FALSE, 0, 0);
		objectData[COLOUR_ID] = NULL;
	}

	// if indices data has been set
	if (indices != NULL)
	{
		// then bind it for use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectIDs[INDICE_ID]);
		// submit data to it
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(*indices).size() * sizeof(GLuint),
			&(*indices)[0],
			GL_STATIC_DRAW
		);

		indexListSize = (*indices).size();
		indices = NULL;
	}



	// Unbind VAO
	glBindVertexArray(NULL);
}

// sets vertex colour data
void GPU_Transfer::setVerticesColour(Vertices& data)
{
	objectData[COLOUR_ID] = &data;
}

// sets texture cords
void GPU_Transfer::setTextureCords(Vertices& data)
{
	objectData[TEXTURE_ID] = &data;
}

// sets vertices
void GPU_Transfer::setVertices(Vertices& data)
{
	objectData[VERTEX_ID] = &data;
}

// sets indices
GLvoid GPU_Transfer::setIndices(Indices& data)
{
	indices = &data;
}

// sets normals
void GPU_Transfer::setNormals(Vertices& data)
{
	objectData[NORMAL_ID] = &data;
}