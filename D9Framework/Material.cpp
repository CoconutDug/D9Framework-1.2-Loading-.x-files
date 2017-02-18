#include "Types.h"
#include "Material.h"

Material::Material() 
	:
	m_diffuse(WHITE),
	m_ambient(WHITE),
	m_specular(WHITE),
	m_emissive(BLACK),
	m_specularPower(8.f)
{

}

Material::Material(const Colour& ambient, const Colour& diffuse, const Colour& specular, float specularPower) 
	:
	m_diffuse(diffuse),
	m_ambient(ambient),
	m_specular(specular),
	m_emissive(BLACK),
	m_specularPower(specularPower)
{

}
