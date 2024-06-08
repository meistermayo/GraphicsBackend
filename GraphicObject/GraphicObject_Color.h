// GraphicObject_Color
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Color
#define _GraphicObject_Color

#include "GraphicObject_Base.h"
#include "Math/Vect.h"

class ShaderColor;

class GraphicObject_Color : public GraphicObject_Base
{
friend class ShaderColor;

public:
	GraphicObject_Color(const GraphicObject_Color&) = delete;				 // Copy constructor
	GraphicObject_Color(GraphicObject_Color&&) = default;                    // Move constructor
	GraphicObject_Color& operator=(const GraphicObject_Color&) & = default;  // Copy assignment operator
	GraphicObject_Color& operator=(GraphicObject_Color&&) & = default;       // Move assignment operator
	~GraphicObject_Color();		  											 // Destructor

	GraphicObject_Color() = delete;

	void SetColor(const Vect& inColor) { Color = inColor; }
	virtual void Render(Camera* inCamera) override;

	GraphicObject_Color(ShaderColor* shader, int meshCount, Model* mod, const Vect& color);
	GraphicObject_Color(Model* mod, ShaderBase* shader, const Vect& color);

private:
	ShaderColor*				pShader;
	Vect							Color;
};

#endif _GraphicObject_Color
