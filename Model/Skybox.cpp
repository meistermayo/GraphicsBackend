#include "Skybox.h"
#include "d3dUtil.h"
#include <cassert>

Skybox::Skybox(ShaderTexture* pShader, Texture* pTexture )
{
	pStdVerts = nullptr;
	pTriList = nullptr;
	numTri = 0;
	numVerts = 0;

	mShader = pShader;
	mTexture = pTexture;

	Skybox::CreateSkybox(pStdVerts,numVerts, pTriList,numTri);
}

Skybox::~Skybox()
{
	delete[] pStdVerts;
	delete[] pTriList;
	delete pModel;

	delete mTexture;
	delete mShader;

	//ReleaseAndDeleteCOMobject(mpVertexBuffer);
	//ReleaseAndDeleteCOMobject(mpIndexBuffer);
}

void Skybox::CreateSkybox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&_pTriList, int& ntri)
{
	nverts = 6*4;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	_pTriList = new TriangleByIndex[ntri];
	//*/
	float scale = 2500.0f;

	//float acrossQ;
	//float acrossH;
	//float across3Q;
	//
	//float downThird;
	//float downThird2;
	//float downThird3;
	float one3 = 0.34f;//1.0f/3.0f;
	float two3 = 0.65f;//2.0f/3.0f;

	pVerts[0].set( -scale, -scale, -scale, 0.0f , two3,  Vect(0,0,0,1), Colors::Black		,0);		// back
	pVerts[1].set( -scale, +scale, -scale, 0.0f , one3,  Vect(0,0,0,1), Colors::Lime		,0);		// back
	pVerts[2].set( +scale, +scale, -scale, 0.25f, one3, Vect(0,0,0,1), Colors::Yellow	,0	);	// back
	pVerts[3].set( +scale, -scale, -scale, 0.25f, two3, Vect(0,0,0,1), Colors::Red		,0	);		// back

	pVerts[4].set( -scale, -scale, +scale, 0.75f, two3, Vect(0,0,0,1), Colors::Blue		,0);		// front
	pVerts[5].set( -scale, +scale, +scale, 0.75f, one3, Vect(0,0,0,1), Colors::Cyan		,0);		// front
	pVerts[6].set( +scale, +scale, +scale, 0.5f, one3,  Vect(0,0,0,1), Colors::White		,0);		// front
	pVerts[7].set( +scale, -scale, +scale, 0.5f, two3,  Vect(0,0,0,1), Colors::Magenta	,0	);	// front

	pVerts[8].set( -scale, -scale, -scale, 1.0f , two3,Vect(0,0,0,1), Colors::Black		,0);	// right ( from cam view) 
	pVerts[9].set( -scale, +scale, -scale, 1.0f , one3,Vect(0,0,0,1), Colors::Lime		,0);	// right ( from cam view) 
	pVerts[10].set(-scale, -scale, +scale, 0.75f, two3, Vect(0,0,0,1), Colors::Blue		,0);		// right ( from cam view) 
	pVerts[11].set(-scale, +scale, +scale, 0.75f, one3, Vect(0,0,0,1), Colors::Cyan		,0);		// right ( from cam view) 

	pVerts[12].set(+scale, +scale, -scale, 0.25f, one3,  Vect(0,0,0,1), Colors::Yellow	,0	);	// left from cam view
	pVerts[13].set(+scale, -scale, -scale, 0.25f, two3,  Vect(0,0,0,1), Colors::Red		,0	);	// left from cam view 
	pVerts[14].set(+scale, +scale, +scale, 0.5f, one3, Vect(0,0,0,1), Colors::White		,0);	// left from cam view 
	pVerts[15].set(+scale, -scale, +scale, 0.5f, two3, Vect(0,0,0,1), Colors::Magenta	,0	);	// left from cam view

	pVerts[16].set(-scale, +scale, -scale, 0.51f, one3,  Vect(0,0,0,1), Colors::Lime		,0);		// top
	pVerts[17].set(+scale, +scale, -scale, 0.749f, one3, Vect(0,0,0,1), Colors::Yellow	,0	);	// top
	pVerts[18].set(+scale, +scale, +scale, 0.749f, 0.0f,  Vect(0,0,0,1), Colors::White		,0);		// top
	pVerts[19].set(-scale, +scale, +scale, 0.51f, 0.0f,   Vect(0,0,0,1), Colors::Cyan		,0);			// top

	pVerts[20].set(-scale, -scale, -scale, 0.51f, two3,  Vect(0,0,0,1), Colors::Black		,0);		// bottom
	pVerts[21].set(+scale, -scale, -scale, 0.749f, two3, Vect(0,0,0,1), Colors::Red		,0	);		// bottom
	pVerts[22].set(-scale, -scale, +scale, 0.51f, 1.0f,  Vect(0,0,0,1), Colors::Blue		,0);		// bottom
	pVerts[23].set(+scale, -scale, +scale, 0.749f, 1.0f, Vect(0,0,0,1), Colors::Magenta	,0	);	// bottom

	// back face
	pTriList[0].set(0, 2, 1);
	pTriList[1].set(0, 3, 2);

	// front face
	pTriList[2].set(4, 5, 6);
	pTriList[3].set(4, 6, 7);

	// left face
	pTriList[4].set(10, 9, 11);
	pTriList[5].set(10, 8, 9);

	// right face
	pTriList[6].set(13, 14, 12);
	pTriList[7].set(13, 15, 14);

	// top face
	pTriList[8].set(16, 18, 19);
	pTriList[9].set(16, 17, 18);

	// bottom face
	pTriList[10].set(22, 21, 20);
	pTriList[11].set(22, 23, 21);
	/*/
	//*/
	pModel = new Model(pVerts, nverts, pTriList, ntri);
}

void Skybox::Render()
{
	mTexture->SetToContext();
	pModel->Render();
}