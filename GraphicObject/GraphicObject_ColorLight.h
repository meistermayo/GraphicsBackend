#ifndef _GraphicObject_Color
#define _GraphicObject_Color

#include "GraphicObject_Base.h"
#include "../Shader/ShaderColor.h"
#include "../Shader/ShaderColorLight.h"

class Vect;

class GraphicObject_ColorLight : public GraphicObject_Base
{
friend class ShaderColor;
friend class ShaderColorLight;

public:
	GraphicObject_ColorLight() = delete;
	~GraphicObject_ColorLight();

	GraphicObject_ColorLight(Model* inMod, ShaderColorLight* inShader, const Vect& inAmb, const Vect& inDif, const Vect& inSp);

	GraphicObject_ColorLight(const GraphicObject_ColorLight&) = delete;					
	GraphicObject_ColorLight(GraphicObject_ColorLight&&) = default;                   
	GraphicObject_ColorLight& operator=(const GraphicObject_ColorLight&) = default; 
	GraphicObject_ColorLight& operator=(GraphicObject_ColorLight&&) = default;      

	void SetColor(const Vect& col, int meshNum);
	virtual void Render(Camera* inCamera) override;

	void SetLights(ShaderColorLight::DirectionalLight* inDirLight, ShaderColorLight::SpotLight* inSptLight, ShaderColorLight::PointLight* inPntLight);

	void SetDirectionalLightParameters(const Vect& inDir, const Vect& inAmb, const Vect& inDif, const Vect& inSp);
	void SetPointLightParameters1(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inAmb, const Vect& inDif, const Vect& inSp);
	void SetPointLightParameters2(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inAmb, const Vect& inDif, const Vect& inSp);
	void SetPointLightParameters3(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inAmb, const Vect& inDif, const Vect& inSp);
	void SetSpotLightParameters(const Vect& inPos, float inR, const Vect& inAtt, const Vect& inDir, float inSpotExp, const Vect& inAmb, const Vect& inDif, const Vect& inSp);

	void SetMaterial(ShaderColorLight::Material* inMaterial);

private:
	ShaderColorLight::DirectionalLight* dirLight;
	ShaderColorLight::SpotLight* sptLight;
	ShaderColorLight::PointLight* pntLight;

	ShaderColorLight* pShader;
	Vect* pColor;
	ShaderColorLight::Material* material;
};

#endif _GraphicObject_Color
