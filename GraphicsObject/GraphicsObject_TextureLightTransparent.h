#pragma once

#ifndef _GraphicsObject_TextureLightTransparent
#define _GraphicsObject_TextureLightTransparent

#include "GraphicsObject_Base.h"
#include "../Math/Vect.h"

class ShaderColorLightTextureTransparent;

class GraphicsObject_TextureLightTransparent : public GraphicsObject_Base
{
	friend class Texture;

public:
	GraphicsObject_TextureLightTransparent() = delete;
	~GraphicsObject_TextureLightTransparent();

	GraphicsObject_TextureLightTransparent(ShaderColorLightTextureTransparent* inShader, Model* inModel, float inAlpha = 0.75f);
	GraphicsObject_TextureLightTransparent(Model* inModel, ShaderColorLightTextureTransparent* inShader, Texture* inTexture, const Vect& inAmb, const Vect& inDif, float inAlpha = 0.75f);

	GraphicsObject_TextureLightTransparent(const GraphicsObject_TextureLightTransparent&) = delete;
	GraphicsObject_TextureLightTransparent(GraphicsObject_TextureLightTransparent&&) = default;
	GraphicsObject_TextureLightTransparent& operator=(const GraphicsObject_TextureLightTransparent&)& = default;
	GraphicsObject_TextureLightTransparent& operator = (GraphicsObject_TextureLightTransparent&&) & = default;

	void SetTexture(Texture* inTex, int i);
	void SetAlpha(float inAlpha);
	virtual void Render(Camera* inCamera) override;

private:
	Texture** pTex;
	ShaderColorLightTextureTransparent* pShader;
	Vect mAmbColor;
	Vect mDifColor;
	float alpha;
};

#endif _GraphicsObject_Texture