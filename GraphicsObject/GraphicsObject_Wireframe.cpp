#include <assert.h>

#include "GraphicsObject_Wireframe.h"
#include "../Camera.h"
#include "../GraphicsBackend.h"
#include "../Model/Model.h"
#include "../Shader/ShaderWireframe.h"

GraphicsObject_Wireframe::GraphicsObject_Wireframe(Model* inModel, ShaderWireframe* inShader, const Vect& inColor)
{
	pModel = inModel;
	pShader = inShader;

	mColor = inColor;
	mWorld = Matrix::Identity;
}

GraphicsObject_Wireframe::~GraphicsObject_Wireframe()
{

}

void GraphicsObject_Wireframe::Render(Camera* inCamera)
{
	GraphicsBackend::SetDrawModeWireframe();

	pModel->BindVertexIndexBuffers();
	pShader->SendWorldColor(mWorld, mColor);
	pShader->SendCamMatrices(inCamera->getViewMatrix(), inCamera->getProjMatrix());
	pShader->SetToContext();
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pModel->RenderMesh(i);
	}

	GraphicsBackend::SetDrawModeFill();
}
