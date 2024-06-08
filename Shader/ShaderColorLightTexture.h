// ShaderColorLight
// Andre Berthiaume, Feb 2017

#ifndef _ShaderColorLightTexture
#define _ShaderColorLightTexture

#include "ShaderBase.h"
#include "Math/Matrix.h"

template <typename T>
struct GenericBufferObject;

class ShaderColorLightTexture : public ShaderBase
{
public:
	ShaderColorLightTexture(const ShaderColorLightTexture&) = delete;				 // Copy constructor
	ShaderColorLightTexture(ShaderColorLightTexture&&) = default;                    // Move constructor
	ShaderColorLightTexture& operator=(const ShaderColorLightTexture&) & = default;  // Copy assignment operator
	ShaderColorLightTexture& operator=(ShaderColorLightTexture&&) & = default;       // Move assignment operator
	~ShaderColorLightTexture();		  				
	ShaderColorLightTexture() = delete;
	ShaderColorLightTexture(std::string filename);

	virtual void SetToContext() override;

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters1(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters2(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters3(const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetSpotLightParameters(const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendLightParameters(const Vect& eyepos);
	void SendWorldAndMaterial(const Matrix& world, const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f));
	void SendFogData(const float& fogMin, const float& fogMax, const Vect& fogCol);

private:
	struct Material
	{
		Vect Ambient{ 1.f, 1.f, 0.f, 1.f };
		Vect Diffuse{ 1.f, 1.f, 0.f, 1.f };
		Vect Specular{ 1.f, 1.f, 0.f, 1.f };
	};

	struct PhongADS
	{
		Vect Ambient{ 1.f, 1.f, 0.f, 1.f };
		Vect Diffuse{ 1.f, 1.f, 0.f, 1.f };
		Vect Specular{ 1.f, 1.f, 0.f, 1.f };
	};

	struct DirectionalLight
	{
		PhongADS Light;
		Vect Direction{ 1.f, 1.f, 0.f, 1.f };
	};

	DirectionalLight DirLightData;

	struct PointLight
	{
		PhongADS Light;
		Vect Position{ 1.f, 1.f, 0.f, 1.f };
		Vect Attenuation{ 1.f, 1.f, 0.f, 1.f };
		float Range = 1.0f;
	};

	PointLight PointLightData1;
	PointLight PointLightData2;
	PointLight PointLightData3;

	struct SpotLight
	{
		PhongADS Light;
		Vect Position{ 1.f, 1.f, 0.f, 1.f };
		Vect Attenuation{ 1.f, 1.f, 0.f, 1.f };
		Vect Direction{ 1.f, 1.f, 0.f, 1.f };
		float SpotExp = 1.0f;
		float Range = 1.0f;
	};

	SpotLight SpotLightData;

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	GenericBufferObject<CamMatrices>*  mpBufferCamMatrices;

	struct Data_WorldAndMaterial
	{
		Matrix World;
		Matrix WorlInv;
		Material Mat;
	};

	GenericBufferObject<Data_WorldAndMaterial>*	mpBuffWordAndMaterial;

	struct Data_LightParams
	{
		DirectionalLight DirLight;
		PointLight PntLight1;
		PointLight PntLight2;
		PointLight PntLight3;
		SpotLight SptLight;
		Vect EyePosWorld;
	};

	GenericBufferObject<Data_LightParams>*  mpBufferLightParams;

	public:
		struct FogData {
			float fogMin = 1000.0f;
			float fogMax = 10000.0f;
			Vect fogCol{ 1.0f, 0.0f, 1.0f, 1.0f };
		};

private:
	GenericBufferObject<FogData>* mpFog;
};

#endif _ShaderColorLight

