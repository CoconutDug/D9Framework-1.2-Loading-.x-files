#pragma once
#include "Types.h"

const Colour WHITE(1.f, 1.f, 1.f, 1.f);
const Colour BLACK(0.f, 0.f, 0.f, 0.f);

class Material
{
public:
	Material();
	Material(const Colour& ambient, const Colour& diffuse, const Colour& specular, float specularPower);
	~Material() {}
	inline operator D3DMATERIAL9*()
	{
		return (D3DMATERIAL9*)(this);
	}
	Colour m_diffuse;
	Colour m_ambient;
	Colour m_specular;
	Colour m_emissive;
	float m_specularPower;
};