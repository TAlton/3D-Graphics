#include "IRenderable.h"



IRenderable::IRenderable()
{
}


IRenderable::~IRenderable()
{
}

void IRenderable::Translate(glm::vec3 xform) {

	for (auto& x : m_vecv3Positions) x + xform;

}
