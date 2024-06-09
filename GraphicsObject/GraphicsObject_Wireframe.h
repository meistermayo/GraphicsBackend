// GraphicsObject_Color
// Andre Berthiaume, July 2016

#ifndef _GraphicsObject_Wireframe
#define _GraphicsObject_Wireframe

#include "GraphicsObject_Base.h"
#include "../Math/Vect.h"

class ShaderWireframe;

class GraphicsObject_Wireframe : public GraphicsObject_Base
{
friend class ShaderWireframe;

public:
	GraphicsObject_Wireframe() = delete;
	~GraphicsObject_Wireframe();

	GraphicsObject_Wireframe(Model* inModel, ShaderWireframe* inShader, const Vect& inColor);

	GraphicsObject_Wireframe(const GraphicsObject_Wireframe&) = delete;				
	GraphicsObject_Wireframe(GraphicsObject_Wireframe&&) = default;                   
	GraphicsObject_Wireframe& operator=(const GraphicsObject_Wireframe&) & = default; 
	GraphicsObject_Wireframe& operator=(GraphicsObject_Wireframe&&) & = default;      

	void SetColor(const Vect& inColor) { mColor = inColor; }
	virtual void Render(Camera* inCamera) override;

private:
	ShaderWireframe* pShader;
	Vect mColor;
};

#endif _GraphicsObject_Color
