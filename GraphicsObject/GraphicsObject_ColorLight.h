#ifndef _GraphicsObject_Color
#define _GraphicsObject_Color

#include "GraphicsObject_Base.h"
#include "../Shader/ShaderColor.h"
#include "../Shader/ShaderColorLight.h"

class Vect;

class GraphicsObject_ColorLight : public GraphicsObject_Base
{
friend class ShaderColor;
friend class ShaderColorLight;

public:
	GraphicsObject_ColorLight() = delete;
	~GraphicsObject_ColorLight();

	GraphicsObject_ColorLight(Model* inMod, ShaderColorLight* inShader, const Vect& inAmb, const Vect& inDif, const Vect& inSp);

	GraphicsObject_ColorLight(const GraphicsObject_ColorLight&) = delete;					
	GraphicsObject_ColorLight(GraphicsObject_ColorLight&&) = default;                   
	GraphicsObject_ColorLight& operator=(const GraphicsObject_ColorLight&) = default; 
	GraphicsObject_ColorLight& operator=(GraphicsObject_ColorLight&&) = default;      

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

#endif _GraphicsObject_Color
