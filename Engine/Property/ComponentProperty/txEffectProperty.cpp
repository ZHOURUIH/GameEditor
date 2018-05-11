#include "txEffectProperty.h"

txVector<std::string> txEffectPropertyParticle::mChoices;
txEffectPropertyParticle::txEffectPropertyParticle()
{
	mPropertyName = "Particle";
	mDescription = "СЃзгЬиаЇ";
}
void txEffectPropertyParticle::setRealValue(const std::string& value, txComponentEffect* component)
{
	txComponentTransform* transform = static_cast<txComponentTransform*>(component->getOwner()->getFirstComponentByBaseType(TOSTRING(txComponentTransform)));
	component->createEffect(value, transform->getNode());
}
std::string txEffectPropertyParticle::getRealValue(txComponentEffect* component)
{
	return component->getEffectFileName();
}