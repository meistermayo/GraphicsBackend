#pragma once

#ifndef _GraphicsObject_Texture
#define _GraphicsObject_Texture

#include "GraphicsObject_Base.h"
#include "../Math/Vect.h"
#include "../Shader/ShaderTexture.h"
#include "../Texture/Texture.h"

class GraphicsObject_Texture : public GraphicsObject_Base
{
	friend class Texture;

public:
	GraphicsObject_Texture() = delete;
	~GraphicsObject_Texture();

	GraphicsObject_Texture(Model* inModel, ShaderTexture* inShader);
	GraphicsObject_Texture(Model* inModel, ShaderTexture* inShader, Texture* inTexture);

	GraphicsObject_Texture(const GraphicsObject_Texture&) = delete;
	GraphicsObject_Texture(GraphicsObject_Texture&&) = default;
	GraphicsObject_Texture& operator=(const GraphicsObject_Texture&)& = default;
	GraphicsObject_Texture& operator = (GraphicsObject_Texture&&) & = default;

	void SetTexture(Texture* inTexture, int i);
	virtual void Render(Camera* inCamera) override;

private:
	Texture ** pTex;
	ShaderTexture* pShader;
};

#endif _GraphicsObject_Texture