// GraphicObject_Color
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Wireframe
#define _GraphicObject_Wireframe

#include "GraphicObject_Base.h"
#include "../Math/Vect.h"

class ShaderWireframe;

class GraphicObject_Wireframe : public GraphicObject_Base
{
friend class ShaderWireframe;

public:
	GraphicObject_Wireframe() = delete;
	~GraphicObject_Wireframe();

	GraphicObject_Wireframe(Model* inModel, ShaderWireframe* inShader, const Vect& inColor);

	GraphicObject_Wireframe(const GraphicObject_Wireframe&) = delete;				
	GraphicObject_Wireframe(GraphicObject_Wireframe&&) = default;                   
	GraphicObject_Wireframe& operator=(const GraphicObject_Wireframe&) & = default; 
	GraphicObject_Wireframe& operator=(GraphicObject_Wireframe&&) & = default;      

	void SetColor(const Vect& inColor) { mColor = inColor; }
	virtual void Render(Camera* inCamera) override;

private:
	ShaderWireframe* pShader;
	Vect mColor;
};

#endif _GraphicObject_Color
