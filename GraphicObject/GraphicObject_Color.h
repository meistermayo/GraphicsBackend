// GraphicObject_Color
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Color
#define _GraphicObject_Color

#include "GraphicObject_Base.h"
#include "../Math/Vect.h"

class ShaderColor;

class GraphicObject_Color : public GraphicObject_Base
{
friend class ShaderColor;

public:
	GraphicObject_Color() = delete;
	~GraphicObject_Color();

	GraphicObject_Color(Model* inModel, ShaderColor* inShader, const Vect& inColor);

	GraphicObject_Color(const GraphicObject_Color&) = delete;
	GraphicObject_Color(GraphicObject_Color&&) = default;                    
	GraphicObject_Color& operator=(const GraphicObject_Color&) = default;  
	GraphicObject_Color& operator=(GraphicObject_Color&&) = default;       


	void SetColor(const Vect& inColor) { mColor = inColor; }
	virtual void Render(Camera* inCamera) override;


private:
	ShaderColor* pShader;
	Vect mColor;
};

#endif _GraphicObject_Color
