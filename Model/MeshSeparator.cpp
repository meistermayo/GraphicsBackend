#include "MeshSeparator.h"
#include <assert.h>
#include "Model.h"

// Improtant: the tri index list may be modified (reordered!)
MeshSeparator::MeshSeparator(const StandardVertex *inVerts, TriangleByIndex*& inTriList, int inTriCount)
{
	// create a temporary map to sort the indices
	using KEY = int;
	using MESHINDICES = std::vector<TriangleByIndex>;	
	std::map<KEY, MESHINDICES> meshes;

	// Create the different list of indices for meshes
	for (int i = 0; i < inTriCount; i++)
	{
		TriangleByIndex ind = inTriList[i];

		// triangles in one mesh shouldn't share vertices with another mesh
		assert((inVerts[ind.v0].meshNum == inVerts[ind.v1].meshNum) && (inVerts[ind.v0].meshNum == inVerts[ind.v2].meshNum));

		// crash 1 todo
		KEY key = (KEY)inVerts[ind.v0].meshNum;
		meshes[key].push_back(ind);
	}

	// we save the mesh count for later validation
	meshcount = meshes.size();

	// Reconsruct a new tri index list ordered by mesh number
	TriangleByIndex* templist = new TriangleByIndex[inTriCount];
	meshdata = new MeshIndexData[meshes.size()];
	int offsetval = 0;

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshdata[i].numTri = meshes[i].size();
		meshdata[i].offset = offsetval;

		size_t test = meshes[i].size();

		for (size_t j = 0; j < test; j++)
		{
			templist[offsetval + j] = meshes[i][j];
		}
		
		offsetval += meshes[i].size();
	}

	// crash 2 todo
	delete[] inTriList;		// Delete the original triangle array
	inTriList = templist;	// Replace it with new re-ordered one

}

MeshSeparator::~MeshSeparator()
{
	delete[] meshdata;
}

void MeshSeparator::GetMeshTriCountAndOffset(int inMeshNum, int& outCount, int& outOffset)
{
	assert((inMeshNum >= 0) && (inMeshNum < GetMeshCount()));

	outCount = meshdata[inMeshNum].numTri;
	outOffset = meshdata[inMeshNum].offset;
}

int MeshSeparator::GetMeshCount()
{
	return meshcount;
}

