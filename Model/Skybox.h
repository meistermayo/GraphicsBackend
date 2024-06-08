#ifndef SKYBOX_H
#define SKYBOX_H

#include "Model.h"
#include "ShaderTexture.h"
#include "Texture.h"

class Skybox {

public:
	Skybox(ShaderTexture* pShader, Texture* pTexture);
	~Skybox();
	void CreateSkybox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&_pTriList, int& ntri);
	void Render();

private:
	//void LoadDataToGPU();

private :
	Model * pModel;
	StandardVertex * pStdVerts;
	TriangleByIndex * pTriList;
	int numTri;
	int numVerts;

	ShaderTexture* mShader;
	Texture* mTexture;
};

#endif