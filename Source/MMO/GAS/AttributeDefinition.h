#pragma once
#include "AttributeSet.h"

#include "AttributeDefinition.generated.h"

UCLASS(EditInlineNew, Abstract)
class UAttributeDefinition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual UAttributeSet* CreateAttributeSet() { return nullptr; }
};
