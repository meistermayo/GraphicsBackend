#ifndef _MeshSeperator
#define _MeshSeperator

#include <map>
#include <vector>
struct StandardVertex;
struct TriangleByIndex;



class MeshSeparator
{
private:
	struct MeshIndexData
	{
		int offset;
		int numTri;
	};
	
	MeshIndexData* meshdata;
	int meshcount;

public:
	MeshSeparator(const MeshSeparator&) = delete;				 // Copy constructor
	MeshSeparator(MeshSeparator&&) = default;                    // Move constructor
	MeshSeparator& operator=(const MeshSeparator&) & = default;  // Copy assignment operator
	MeshSeparator& operator=(MeshSeparator&&) & = default;       // Move assignment operator
	~MeshSeparator();									 // Destructor

	MeshSeparator() = delete;
	MeshSeparator(const StandardVertex *inVerts, TriangleByIndex*& inTriList, int inTriCount);

	int GetMeshCount();
	void GetMeshTriCountAndOffset(int inMeshNum, int& outCount, int& outOffset);

};

#endif _MeshSeperator

