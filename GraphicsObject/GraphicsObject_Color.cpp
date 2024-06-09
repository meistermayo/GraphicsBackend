#include "GraphicsObject_Color.h"
#include "../Camera.h"
#include "../Model/Model.h"
#include "../Shader/ShaderColor.h"
#include <assert.h>

GraphicsObject_Color::GraphicsObject_Color(Model* inModel, ShaderColor* inShader, const Vect& inColor)
{
	pModel = inModel;
	pShader = inShader;
	mColor = inColor;

	mWorld = Matrix::Identity;
}

// NOTE can prob get rid of ptrs to lights...
GraphicsObject_Color::~GraphicsObject_Color()
{

}

void GraphicsObject_Color::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldColor(mWorld, mColor);
		pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
		pShader->SetToContext();
		pModel->RenderMesh(i);
	}
}
