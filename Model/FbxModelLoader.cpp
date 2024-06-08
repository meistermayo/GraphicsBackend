#include "FbxModelLoader.h"
#include "Math/Constants.h"
#include "Model.h"
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>

FbxModelLoader::FbxModelLoader()
{

}

FbxModelInfo FbxModelLoader::GetModel(const char* fileName)
{
	FbxManager* pManager = FbxManager::Create();
	FbxModelInfo fbxModelInfo = FbxModelInfo();

	FbxIOSettings * ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	FbxImporter* pImporter = FbxImporter::Create(pManager, "");

	if (!pImporter->Initialize(fileName, -1, pManager->GetIOSettings()))
	{
		assert(false);
	}
	else
	{
		// ???
		FbxScene * pScene = FbxScene::Create(pManager, "uhhh");

		pImporter->Import(pScene);
		pImporter->Destroy();


		FbxNode* pRootNode = pScene->GetRootNode();
		if (pRootNode)
		{
			for (int n = 0; n < pRootNode->GetChildCount(); n++)
			{
				FbxMesh* mesh = pRootNode->GetChild(n)->GetMesh();
				FbxMeshInfo meshInfo;

				if (mesh)
				{
					mesh->GenerateNormals(true, true);
					FbxGeometryElementNormal* pNormalElement = mesh->GetElementNormal();
					FbxGeometryElementUV* pUVElement = mesh->GetElementUV(0);
					FbxVector4* pControlPoints = mesh->GetControlPoints();

					meshInfo.nTris = mesh->GetPolygonCount();
					meshInfo.nVerts = meshInfo.nTris * 3;

					int polygonCount = mesh->GetPolygonCount();
					std::vector<int> indexVector;

					std::vector<FbxVector4> _normals;
					
					for (int i = 0; i < mesh->GetPolygonCount(); i++)
					{
						_normals.push_back(pNormalElement->GetDirectArray().GetAt(i));
					}

					meshInfo.pVertices = new StandardVertex[polygonCount * 3];
					for (int i = 0; i < polygonCount; i++)
					{
						int indexCount = mesh->GetPolygonSize(i);
						assert(indexCount == 3);
						for (int j = 0; j < 3; j++)
						{
							int k = i * 3 + j;
							int ind = mesh->GetPolygonVertex(i, j);

							FbxVector4 controlPoint = pControlPoints[ind];

							// These may require a smarter solution soon
							FbxVector4 normal = GetLayerElement<FbxVector4>(pNormalElement, ind);
							FbxVector2 uv = GetLayerElement<FbxVector2>(pUVElement, k);

							Vect controlVect = Vect(
								static_cast<float>(controlPoint[0]),
								static_cast<float>(controlPoint[1]),
								static_cast<float>(controlPoint[2])
							) * Matrix::RotX(-MATH_PI * 0.5f);

							Vect normalVect = Vect(
								static_cast<float>(normal[0]),
								static_cast<float>(normal[1]),
								static_cast<float>(normal[2]),
								static_cast<float>(normal[3])
							) * Matrix::RotX(-MATH_PI * 0.5f);

							meshInfo.pVertices[i*3+j].set(
								controlVect.x,
								controlVect.y,
								controlVect.z,
								static_cast<float>(uv[0]),
								1.0f - static_cast<float>(uv[1]),
								normalVect,
								Vect(1, 1, 1),
								0
							);
							indexVector.push_back(ind);
						}
					}

					meshInfo.pTris = new TriangleByIndex[mesh->GetPolygonCount()];
					for (int i = 0; i < mesh->GetPolygonCount(); i++)
					{
						meshInfo.pTris[i].set (i*3, i*3 + 1, i*3 + 2);
					}

					fbxModelInfo.meshInfo.push_back(meshInfo);
				}
			}
		}
	}

	pManager->Destroy();
	return fbxModelInfo;
}

StandardVertex * FbxModelLoader::GetVertices(FbxMesh * pMesh)
{
	// int count = pMesh->GetUVLayerCount();

	pMesh->GenerateNormals(true, true);
	int controlPointsCount = pMesh->GetControlPointsCount();// "points... "
	int polygonVertexCount = pMesh->GetPolygonVertexCount(); // but not vertices.... danmb...
	int vertexCount = polygonVertexCount > controlPointsCount ? polygonVertexCount : controlPointsCount;

	StandardVertex* verts = new StandardVertex[controlPointsCount];

	FbxGeometryElementNormal* pNormalElement = pMesh->GetElementNormal();
	FbxGeometryElementUV* pUVElement = pMesh->GetElementUV(0);
	// int* vertexArray = pMesh->GetPolygonVertices();
	Vect* normals = new Vect[vertexCount];
	Vect* uvs = new Vect[vertexCount];

	// how do we know how to loop this?...
	switch (pNormalElement->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		{
			for (int i = 0; i < controlPointsCount; i++)
			{
				FbxVector4 normal = GetLayerElement<FbxVector4>(pNormalElement, i);
				normals[i] = Vect(
					static_cast<float>(normal[0]),
					static_cast<float>(normal[1]),
					static_cast<float>(normal[2]),
					static_cast<float>(normal[3])
				);
			}
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		{
			for (int i = 0; i < polygonVertexCount; i++)
			{
				FbxVector4 normal = GetLayerElement<FbxVector4>(pNormalElement, i);
				normals[i] = Vect(
					static_cast<float>(normal[0]),
					static_cast<float>(normal[1]),
					static_cast<float>(normal[2]),
					static_cast<float>(normal[3])
				);
			}
		}
		break;
	}

	// uvs
	switch (pUVElement->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		for (int i = 0; i < controlPointsCount; i++)
		{
			FbxVector2 uv = GetLayerElement<FbxVector2>(pUVElement, i);
			uvs[i] = Vect(
				static_cast<float>(uv[0]),
				static_cast<float>(uv[1]),
				0.0f
			);
		}
	}
	break;
	case FbxGeometryElement::eByPolygonVertex:
	{
		for (int i = 0; i < polygonVertexCount; i++)
		{
			FbxVector2 uv = GetLayerElement<FbxVector2>(pUVElement, i);
			uvs[i] = Vect(
				static_cast<float>(uv[0]),
				static_cast<float>(uv[1]),
				0.0f
			);
		}
	}
	break;
	}

	// verts
	FbxDouble scale = 1.0f;
	for (int i = 0; i < controlPointsCount; i++)
	{
		FbxVector4 controlPoint = pMesh->GetControlPointAt(i);
		verts[i] = StandardVertex();
		verts[i].set(
			static_cast<float>(controlPoint.mData[0] * scale),
			static_cast<float>(controlPoint.mData[1] * scale),
			static_cast<float>(controlPoint.mData[2] * scale),
			static_cast<float>(uvs[i].x),
			static_cast<float>(uvs[i].y),
			normals[i],
			Vect(1,1,1),
			0
		);
	}

	return verts;
}

template <typename T>
T FbxModelLoader::GetLayerElement(FbxLayerElementTemplate<T>* pLayerElement, int index)
{
	switch (pLayerElement->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (pLayerElement->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				return pLayerElement->GetDirectArray().GetAt(index);
			}
			break;
		case FbxGeometryElement::eIndexToDirect:
			{
				int subIndex = pLayerElement->GetIndexArray().GetAt(index);
				return pLayerElement->GetDirectArray().GetAt(subIndex);
			}
			break;
		}

		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (pLayerElement->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			{
				return pLayerElement->GetDirectArray().GetAt(index);
			}
			break;
		case FbxGeometryElement::eIndexToDirect:
			{
				int subIndex = pLayerElement->GetIndexArray().GetAt(index);
				return pLayerElement->GetDirectArray().GetAt(subIndex);
			}
			break;
		}
		break;
	}

	return T();
}

TriangleByIndex * FbxModelLoader::GetTriangles(FbxMesh * pMesh)
{
	int polygonCount = pMesh->GetPolygonCount();
	std::vector<TriangleByIndex*> triVector = std::vector<TriangleByIndex*>();

	for (int i = 0; i < polygonCount; i++)
	{
		int indexCount = pMesh->GetPolygonSize(i);
		assert(indexCount == 3);
		triVector.push_back(new TriangleByIndex());
		int tri[3];
		for (int j=0; j<3; j++)
		{
			tri[j] = pMesh->GetPolygonVertex(i, j);
		}

		triVector[triVector.size() - 1]->set(tri[0], tri[1], tri[2]);
	}
	TriangleByIndex* tris = new TriangleByIndex[triVector.size()];

	for (size_t i = 0; i < triVector.size(); i++)
	{
		tris[i] = *triVector[i];
	}

	return tris;
}
