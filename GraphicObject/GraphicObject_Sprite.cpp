#include "GraphicObject_Sprite.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Model.h"
#include "src/Graphics/CrazySpaceMeatLand/src/ShaderTexture.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Image.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Math/Matrix.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Camera.h"

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
