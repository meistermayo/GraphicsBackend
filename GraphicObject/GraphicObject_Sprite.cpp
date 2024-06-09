#include "GraphicObject_Sprite.h"

#include "../Camera.h"
#include "../Math/Matrix.h"
#include "../Model/Model.h"
#include "../Shader/ShaderTexture.h"
#include "../Texture/Image.h"

GraphicObject_Sprite::GraphicObject_Sprite(Model* inModel, ShaderTexture* inShader, Image* inImage, Rect* inRect)
{
	pModel = inModel;
	pShader = inShader;
	pImage = inImage;

	origPosX = inRect->x;
	origPosY = inRect->y;
	origWidth = inRect->width;
	origHeight = inRect->height;

	mWorld = Matrix::Identity;
}

GraphicObject_Sprite::~GraphicObject_Sprite()
{
}

void GraphicObject_Sprite::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	pShader->SendWorld(mWorld);
	pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
	pShader->SetToContext();

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pModel->RenderMesh(i);
	}
}
