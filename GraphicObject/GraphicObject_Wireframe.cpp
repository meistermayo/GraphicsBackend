#include <assert.h>

#include "GraphicObject_Wireframe.h"
#include "../Camera.h"
#include "../Model/Model.h"
#include "../Shader/ShaderWireframe.h"

GraphicObject_Wireframe::GraphicObject_Wireframe(Model* inModel, ShaderWireframe* inShader, const Vect& inColor)
{
	pModel = inModel;
	pShader = inShader;

	mColor = inColor;
	mWorld = Matrix::Identity;
}

GraphicObject_Wireframe::~GraphicObject_Wireframe()
{

}

void GraphicObject_Wireframe::Render(Camera* inCamera)
{
	pModel->BindVertexIndexBuffers();
	pShader->SendWorldColor(mWorld, mColor);
	pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
	pShader->SetToContext();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pModel->RenderMesh(i);
	}
}
