#pragma once

#ifndef _GraphicObject_TextureL
#define _GraphicObject_TextureL

#include "GraphicObject_Base.h"
#include "../Math/Vect.h"

class ShaderColorLightTexture;

class GraphicObject_TextureLight : public GraphicObject_Base
{
	friend class Texture;

public:
	GraphicObject_TextureLight() = delete;
	~GraphicObject_TextureLight();

	GraphicObject_TextureLight(ShaderColorLightTexture* inShader, Model* inModel);
	GraphicObject_TextureLight(Model* inModel, ShaderColorLightTexture* inShader, Texture* inTexture, const Vect& inAmb, const Vect& inDif);

	GraphicObject_TextureLight(const GraphicObject_TextureLight&) = delete;
	GraphicObject_TextureLight(GraphicObject_TextureLight&&) = default;
	GraphicObject_TextureLight& operator=(const GraphicObject_TextureLight&)& = default;
	GraphicObject_TextureLight& operator = (GraphicObject_TextureLight&&) & = default;

	void SetTexture(Texture* inTex, int i);
	virtual void Render(Camera* inCamera) override;

private:
	Texture ** pTex;
	ShaderColorLightTexture* pShader;
	Vect mAmbColor;
	Vect mDifColor;
};

#endif _GraphicObject_Texture