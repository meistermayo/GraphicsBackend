#ifndef GRAPHICS_OBJECT_SPRITE_H
#define GRAPHICS_OBJECT_SPRITE_H

#include "GraphicObject_Base.h"
#include "../Texture/Texture.h"
#include "../Texture/Image.h"

class Camera;
class ShaderTexture;

class GraphicObject_Sprite : public GraphicObject_Base
{
public:
	GraphicObject_Sprite() = delete;
	~GraphicObject_Sprite();

	GraphicObject_Sprite(Model* inModel, ShaderTexture* inShader, Image* inImage, Rect* inRect);

	GraphicObject_Sprite(const GraphicObject_Sprite&) = delete;
	GraphicObject_Sprite(GraphicObject_Sprite&&) = default;
	GraphicObject_Sprite& operator=(const GraphicObject_Sprite&) = default;
	GraphicObject_Sprite& operator=(GraphicObject_Sprite&&) = default;

	virtual void Render(Camera* inCamera) override;

	Image* pImage;

	float origWidth = 0.0f;
	float origHeight = 0.0f;
	
	float origPosX = 0.0f;
	float origPosY = 0.0f;

private:
	Model* pModel;
	ShaderTexture* pShader;
};

#endif