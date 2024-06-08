#include "GraphicObject_Color.h"
#include "Model.h"
#include "ShaderColor.h"
#include <assert.h>
#include "src/Graphics/CrazySpaceMeatLand/src/Camera.h"

GraphicObject_Color::GraphicObject_Color(ShaderColor* shader, int meshCount, Model* mod, const Vect& color)
{
	meshCount; // unreferenced fopr now

	SetModel(mod);
	pShader = (ShaderColor*)shader;

	Color = color;
	pWorld = new Matrix(Matrix::Identity);
}

GraphicObject_Color::GraphicObject_Color(Model* mod, ShaderBase* shader, const Vect& color)
{
	SetModel(mod);
	pShader = (ShaderColor*)shader;

	Color = color;
	pWorld = new Matrix(Matrix::Identity);
}

// NOTE can prob get rid of ptrs to lights...
GraphicObject_Color::~GraphicObject_Color()
{

}

void GraphicObject_Color::Render(Camera* inCamera)
{
	//pShader->SendWorldAndMaterial(World,material->Ambient,material->Diffuse,material->Specular); 
	pModel->BindVertexIndexBuffers();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldColor(*pWorld, Color);
		pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
		pShader->SetToContext();
		pModel->RenderMesh(i);
	}
}
