#ifndef _GraphicsObject_Color
#define _GraphicsObject_Color

#include "GraphicsObject_Base.h"
#include "../Math/Vect.h"

class ShaderColor;

class GraphicsObject_Color : public GraphicsObject_Base
{
friend class ShaderColor;

public:
	GraphicsObject_Color() = delete;
	~GraphicsObject_Color();

	GraphicsObject_Color(Model* inModel, ShaderColor* inShader, const Vect& inColor);

	GraphicsObject_Color(const GraphicsObject_Color&) = delete;
	GraphicsObject_Color(GraphicsObject_Color&&) = default;                    
	GraphicsObject_Color& operator=(const GraphicsObject_Color&) = default;  
	GraphicsObject_Color& operator=(GraphicsObject_Color&&) = default;       


	void SetColor(const Vect& inColor) { mColor = inColor; }
	virtual void Render(Camera* inCamera) override;


private:
	ShaderColor* pShader;
	Vect mColor;
};

#endif _GraphicsObject_Color
