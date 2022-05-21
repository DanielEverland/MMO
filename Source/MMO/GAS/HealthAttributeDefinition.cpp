#include "HealthAttributeDefinition.h"

#include "HealthAttributeSet.h"

UAttributeSet* UHealthAttributeDefinition::CreateAttributeSet()
{
	const TObjectPtr<UHealthAttributeSet> NewAttribute = NewObject<UHealthAttributeSet>();
	NewAttribute->InitHealth(StartHealth);
	NewAttribute->InitMaxHealth(MaxHealth);

	return NewAttribute;
}
