#ifndef GRAPHICS_OBJECT_SPRITE_H
#define GRAPHICS_OBJECT_SPRITE_H

#include <sb6.h>
#include "GraphicObject_Base.h"
#include "Texture.h"
#include "Image.h"

class Camera;
class ShaderBase;

class GraphicObject_Sprite : public GraphicObject_Base
{
public:
	GraphicObject_Sprite(Model* model, ShaderBase* pShaderObj, Image* image, Rect* rect);
	~GraphicObject_Sprite();

//protected:
	// Rendermaterial contract
	// virtual void privSetState() override;
	// virtual void privSetDataGPU(Camera* pCam) override;
	// virtual void privDraw() override;
	// virtual void privRestoreState() override;

	virtual void Render(Camera* inCamera) override;

public:
	// data:  place uniform instancing here

	//GLuint textureID;
	Image* pImage;

	float origWidth = 0.0f;
	float origHeight = 0.0f;
	
	float origPosX = 0.0f;
	float origPosY = 0.0f;

private:
	// Matrix* pMatrix_uv;
	// Matrix* pMatrix_orig;

	Model* pModel;
	ShaderBase* pShader;
};

#endif