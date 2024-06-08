#include <assert.h>
#include "File.h"
#include "Model.h"
#include "ModelTools.h"
#include "d3dUtil.h"
#include "AzulFileHdr.h"
#include "MeshSeparator.h"
#include "GraphicsBackend.h"

Model::Model(StandardVertex *pVerts, int nverts, TriangleByIndex *ptlist, int ntri)
{
	assert(nverts > 0);
	assert(ntri > 0);

	// Copy Data
	numVerts = nverts;
	pStdVerts = new StandardVertex[numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		pStdVerts[i] = pVerts[i];
	}
	numTris = ntri;
	pTriList = new TriangleByIndex[numTris];
	for (int i = 0; i < numTris; i++)
	{
		pTriList[i] = ptlist[i];
	}

	meshes = new MeshSeparator(pStdVerts, numVerts, pTriList, numTris);
	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::Model(const char * const _modelName, bool flipU, bool flipV, float scale)
{
	pStdVerts = nullptr;
	pTriList = nullptr;
	numVerts = 0;
	numTris = 0;

	privLoadDataFromFile(_modelName, pStdVerts, numVerts, pTriList, numTris, flipU, flipV, scale );
	meshes = new MeshSeparator(pStdVerts, numVerts, pTriList, numTris);
	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::Model(FbxMeshInfo& fbxMeshInfo)
{
	numVerts = fbxMeshInfo.nVerts;
	numTris = fbxMeshInfo.nTris;

	pStdVerts = fbxMeshInfo.pVertices;
	pTriList = fbxMeshInfo.pTris;
	
	meshes = new MeshSeparator(pStdVerts, numVerts, pTriList, numTris);
	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::Model(Model::PreMadeModels pm, float scale)
{
	pStdVerts = nullptr;
	pTriList = nullptr;
	numVerts = 0;
	numTris = 0;

	switch (pm)
	{
	case UnitBox:
		ModelTools::CreateUnitBox(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitBoxRepeatedTexture:
		ModelTools::CreateUnitBoxRepTexture(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitPyramid:
		ModelTools::CreateUnitPyramid(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitSphere:
		ModelTools::CreateUnitSphere(12, 12, pStdVerts, numVerts, pTriList, numTris, scale);
		break;
	case UnitPlaneXY:
		ModelTools::CreateUnitPlaneXY(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitPlaneXZ:
		ModelTools::CreateUnitPlaneXZ(pStdVerts, numVerts, pTriList, numTris);
		break;
	default:
		assert(false && "Invalid option");
	}

	meshes = new MeshSeparator(pStdVerts, numVerts, pTriList, numTris);

	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::~Model()
{
	delete[] pTriList;
	delete[] pStdVerts;
	delete[] pVectList;

	delete mpVertexBufferObject;
	delete mpIndexBufferObject;
}

void Model::privLoadDataFromFile(const char * const _modelName, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& ptlist, int& ntri, bool flipU, bool flipV, float scale)
{
	// Read from file
	FileHandle fh;
	FileError  ferror;

	ferror = File::open(fh, _modelName, FILE_READ);
	assert(ferror == FILE_SUCCESS);

	// GEt the file format for the data
	AzulFileHdr  azulFileHdr;

	// **** Overly complicated detour Part 1:  The vertex data in the file is packed efficiently
	// whereas our StandardVertex is not. Something I shall fix form the start next time.
	// For time time: we will read in the data into a temp array and then manually copy the data into
	// the StandardVertex array. 

	struct VertexStride_VUN
	{
		float x;    // Vert - V
		float y;
		float z;

		float u;    // UV tex coor - U
		float v;

		float txt;	// texture trickery 

		float nx;   // Norm verts - N
		float ny;
		float nz;

		float r;
		float g;
		float b;

		void set(float _x, float _y, float _z,
			float _u, float _v,
			float _nx, float _ny, float _nz,
			float _txt = 0,
			float _r = 1, float _g = 1, float _b = 1)
		{
			x = _x;
			y = _y;
			z = _z;
			u = _u;
			v = _v;
			nx = _nx;
			ny = _ny;
			nz = _nz;

			txt = _txt;
			r = _r;
			g = _g;
			b = _b;
		}

	};

	// ******************


	// Read the header
	ferror = File::read(fh, &azulFileHdr, sizeof(AzulFileHdr));
	assert(ferror == FILE_SUCCESS);

	// create the vertex buffer
	nverts = azulFileHdr.numVerts;
	VertexStride_VUN* pTmpVerts = new VertexStride_VUN[nverts];

	// load the verts
	// seek to the location
	ferror = File::seek(fh, FILE_SEEK_BEGIN, azulFileHdr.vertBufferOffset);
	assert(ferror == FILE_SUCCESS);

	// read it
	ferror = File::read(fh, pTmpVerts, nverts * sizeof(VertexStride_VUN));
	assert(ferror == FILE_SUCCESS);

	// create the triLists buffer
	ntri = azulFileHdr.numTriangles;
	ptlist = new TriangleByIndex[ntri];

	// load the triList
	// seek to the location
	ferror = File::seek(fh, FILE_SEEK_BEGIN, azulFileHdr.triangleListBufferOffset);
	assert(ferror == FILE_SUCCESS);

	// read it
	ferror = File::read(fh, ptlist, ntri * sizeof(TriangleByIndex));
	assert(ferror == FILE_SUCCESS);

	// close
	ferror = File::close(fh);
	assert(ferror == FILE_SUCCESS);

	// **** Overly complicated detour Part 2: now we copy the vertex data into our StandardVertex
	pVerts = new StandardVertex[nverts];
	StandardVertex* v;
	VertexStride_VUN* tmp;
	for (int i = 0; i < nverts; i++)
	{
		tmp = &(pTmpVerts[i]);
		v = &(pVerts[i]);
		v->set(tmp->x, tmp->y, tmp->z, tmp->u, tmp->v, tmp->nx, tmp->ny, tmp->nz, tmp->r, tmp->g, tmp->b, (int) tmp->txt);
		if (flipU)
			v->u = -v->u;
		if (flipV)
			v->v = -v->v;
		v->Pos = v->Pos * scale;
	}

	delete[] pTmpVerts;
	
	// *************************

}

void Model::privPopulateHelperData()
{
	pVectList = new Vect[numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		pVectList[i] = pStdVerts[i].Pos;
	}
}

void  Model::privLoadDataToGPU()
{
	mpVertexBufferObject = new VertexBufferObject();
	mpVertexBufferObject->num = numVerts;
	mpVertexBufferObject->pVerts = pStdVerts;
	mpIndexBufferObject = new IndexBufferObject();
	mpIndexBufferObject->num = numTris;
	mpIndexBufferObject->pTris = pTriList;

	mpVertexBufferObject->LoadToGPU();
	mpIndexBufferObject->LoadToGPU();
}

bool Model::ValidMeshNum(int meshnum)
{
	return (meshnum >= 0) && (meshnum < GetMeshCount());
}

int Model::GetMeshCount()
{
	return meshes->GetMeshCount();
}

void  Model::BindVertexIndexBuffers()
{
	mpVertexBufferObject->Bind();
	mpIndexBufferObject->Bind();
	GraphicsBackend::SetPrimitiveTopologyAsTriList();
}

void Model::Render()
{
	BindVertexIndexBuffers();
	GraphicsBackend::DrawIndexed(numTris * 3, 0, 0);
}

void Model::RenderMesh(int meshnum)
{
	assert(ValidMeshNum(meshnum));

	int tricount, trioffset;
	meshes->GetMeshTriCountAndOffset(meshnum, tricount, trioffset);

	GraphicsBackend::DrawIndexed(tricount * 3, trioffset * 3, 0);
}





