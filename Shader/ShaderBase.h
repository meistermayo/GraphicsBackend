#ifndef _ShaderBase
#define _ShaderBase

#include "../Math/Matrix.h"
#include <string>

struct ShaderInterface;

class ShaderBase : public Align16
{
protected:
	void SetToContext_VS_PS_InputLayout();

	ShaderInterface* mpShaderInterface;

public:
	virtual void SetToContext() {};

	ShaderBase(std::string filename);
	ShaderBase(const ShaderBase&) = default;				
	ShaderBase(ShaderBase&&) = delete;						
	ShaderBase& operator=(const ShaderBase&) & = delete;	
	ShaderBase& operator=(ShaderBase&&) & = delete;			
	ShaderBase() = delete;
	virtual ~ShaderBase();
};


#endif _ShaderBase