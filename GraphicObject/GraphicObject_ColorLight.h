#ifndef _GraphicObject_Color
#define _GraphicObject_Color

#include "GraphicObject_Base.h"
#include "Math/Vect.h"
#include "ShaderColor.h"
#include "ShaderColorLight.h"

class GraphicObject_ColorLight : public GraphicObject_Base
{
friend class ShaderColor;
friend class ShaderColorLight;

public:
	GraphicObject_ColorLight(const GraphicObject_ColorLight&) = delete;				 // Copy constructor
	GraphicObject_ColorLight(GraphicObject_ColorLight&&) = default;                    // Move constructor
	GraphicObject_ColorLight& operator=(const GraphicObject_ColorLight&) & = default;  // Copy assignment operator
	GraphicObject_ColorLight& operator=(GraphicObject_ColorLight&&) & = default;       // Move assignment operator
	~GraphicObject_ColorLight();		  											 // Destructor

	GraphicObject_ColorLight() = delete;

	void SetColor(const Vect& col, int meshNum);
	virtual void Render(Camera* inCamera) override;

	void SetLights(ShaderColorLight::DirectionalLight* _dirLight, ShaderColorLight::SpotLight* _sptLight, ShaderColorLight::PointLight* _pntLight);

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp);
	void SetPointLightParameters1(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp);
	void SetPointLightParameters2(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp);
	void SetPointLightParameters3(const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp);
	void SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp);

	void SetMaterial(ShaderColorLight::Material* _material);

	GraphicObject_ColorLight(ShaderColorLight* shader, int meshCount, Model* mod, const Vect &amb, const Vect &dif, const Vect& sp);

private:
	ShaderColorLight::DirectionalLight* dirLight;
	ShaderColorLight::SpotLight* sptLight;
	ShaderColorLight::PointLight* pntLight;

	ShaderColorLight*					pShader;
	Vect*							Color;
	ShaderColorLight::Material* material;
};

#endif _GraphicObject_Color
