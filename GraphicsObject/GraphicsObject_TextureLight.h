#pragma once

#ifndef _GraphicsObject_TextureL
#define _GraphicsObject_TextureL

#include "GraphicsObject_Base.h"
#include "../Math/Vect.h"

class ShaderColorLightTexture;

class GraphicsObject_TextureLight : public GraphicsObject_Base
{
	friend class Texture;

public:
	GraphicsObject_TextureLight() = delete;
	~GraphicsObject_TextureLight();

	GraphicsObject_TextureLight(ShaderColorLightTexture* inShader, Model* inModel);
	GraphicsObject_TextureLight(Model* inModel, ShaderColorLightTexture* inShader, Texture* inTexture, const Vect& inAmb, const Vect& inDif);

	GraphicsObject_TextureLight(const GraphicsObject_TextureLight&) = delete;
	GraphicsObject_TextureLight(GraphicsObject_TextureLight&&) = default;
	GraphicsObject_TextureLight& operator=(const GraphicsObject_TextureLight&)& = default;
	GraphicsObject_TextureLight& operator = (GraphicsObject_TextureLight&&) & = default;

	void SetTexture(Texture* inTex, int i);
	virtual void Render(Camera* inCamera) override;

private:
	Texture ** pTex;
	ShaderColorLightTexture* pShader;
	Vect mAmbColor;
	Vect mDifColor;
};

#endif _GraphicsObject_Texture