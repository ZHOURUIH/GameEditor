#ifndef _TX_EFFECT_PROPERTY_H_
#define _TX_EFFECT_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentEffect)

class txEffectPropertyParticle : public txComponentEffectProperty<txStringProperty, std::string>
{
public:
	txEffectPropertyParticle();
	virtual void setRealValue(const std::string& value, txComponentEffect* component);
	virtual std::string getRealValue(txComponentEffect* component);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};
#endif
