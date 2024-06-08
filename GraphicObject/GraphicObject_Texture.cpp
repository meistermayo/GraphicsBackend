#include "GraphicObject_Texture.h"
#include "Model.h"
#include <assert.h>
#include "src/Graphics/CrazySpaceMeatLand/src/Camera.h"
#include "src/Graphics/CrazySpaceMeatLand/src/Camera.h"

GraphicObject_Texture::GraphicObject_Texture(ShaderTexture* shader,Model* mod)
{
	pShader = shader;
	SetModel(mod);
	tex = new Texture*[mod->GetMeshCount()];
	pWorld = new Matrix(Matrix::Identity);
}

GraphicObject_Texture::GraphicObject_Texture(Model* mod, ShaderTexture* shader, Texture* texture)
{
	pShader = shader;
	SetModel(mod);
	tex = new Texture * [mod->GetMeshCount()];
	tex[0] = texture;
	pWorld = new Matrix(Matrix::Identity);
}

GraphicObject_Texture::~GraphicObject_Texture() {
	delete tex;
}

void GraphicObject_Texture::SetTexture(Texture* _tex, int i)
{
	assert(pModel->ValidMeshNum(i));
	this->tex[i] = _tex;
}

void GraphicObject_Texture::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		tex[i]->SetToContext();
		pShader->SetToContext();
		pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
		pShader->SendWorld(*pWorld);
		pModel->RenderMesh(i);
	}
}