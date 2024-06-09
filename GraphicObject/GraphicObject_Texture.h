#pragma once

#ifndef _GraphicObject_Texture
#define _GraphicObject_Texture

#include "GraphicObject_Base.h"
#include "../Math/Vect.h"
#include "../Shader/ShaderTexture.h"
#include "../Texture/Texture.h"

class GraphicObject_Texture : public GraphicObject_Base
{
	friend class Texture;

public:
	GraphicObject_Texture() = delete;
	~GraphicObject_Texture();

	GraphicObject_Texture(Model* inModel, ShaderTexture* inShader);
	GraphicObject_Texture(Model* inModel, ShaderTexture* inShader, Texture* inTexture);

	GraphicObject_Texture(const GraphicObject_Texture&) = delete;
	GraphicObject_Texture(GraphicObject_Texture&&) = default;
	GraphicObject_Texture& operator=(const GraphicObject_Texture&)& = default;
	GraphicObject_Texture& operator = (GraphicObject_Texture&&) & = default;

	void SetTexture(Texture* inTexture, int i);
	virtual void Render(Camera* inCamera) override;

private:
	Texture ** pTex;
	ShaderTexture* pShader;
};

#endif _GraphicObject_Texture