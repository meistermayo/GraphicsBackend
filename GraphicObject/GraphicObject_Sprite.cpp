#include "GraphicObject_Sprite.h"

#include "../Camera.h"
#include "../Math/Matrix.h"
#include "../Model/Model.h"
#include "../Shader/ShaderTexture.h"
#include "../Texture/Image.h"

GraphicObject_Sprite::GraphicObject_Sprite(Model* model, ShaderBase* shader, Image* image, Rect* rect)
{
	pWorld = new Matrix(Matrix::Identity);
	pModel = model;
	pShader = shader;
	pImage = image;
	rect;
	// rect?...
}

GraphicObject_Sprite::~GraphicObject_Sprite()
{
	delete pWorld;
}

void GraphicObject_Sprite::Render(Camera* inCamera)
{
	//pShader->SendWorldAndMaterial(World,material->Ambient,material->Diffuse,material->Specular); 
	pModel->BindVertexIndexBuffers();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		((ShaderTexture*)pShader)->SendWorld(*pWorld);
		((ShaderTexture*)pShader)->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
		pShader->SetToContext();
		pModel->RenderMesh(i);
	}
}
