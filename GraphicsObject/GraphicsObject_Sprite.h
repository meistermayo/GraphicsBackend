#ifndef GRAPHICS_OBJECT_SPRITE_H
#define GRAPHICS_OBJECT_SPRITE_H

#include "GraphicsObject_Base.h"
#include "../Texture/Texture.h"
#include "../Texture/Image.h"

class Camera;
class ShaderTexture;

class GraphicsObject_Sprite : public GraphicsObject_Base
{
public:
	GraphicsObject_Sprite() = delete;
	~GraphicsObject_Sprite();

	GraphicsObject_Sprite(Model* inModel, ShaderTexture* inShader, Image* inImage, Rect* inRect);

	GraphicsObject_Sprite(const GraphicsObject_Sprite&) = delete;
	GraphicsObject_Sprite(GraphicsObject_Sprite&&) = default;
	GraphicsObject_Sprite& operator=(const GraphicsObject_Sprite&) = default;
	GraphicsObject_Sprite& operator=(GraphicsObject_Sprite&&) = default;

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