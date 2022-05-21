#pragma once

#include "AttributeDefinition.h"
#include "HealthAttributeDefinition.generated.h"

UCLASS(EditInlineNew)
class UHealthAttributeDefinition : public UAttributeDefinition
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere)
	float StartHealth = 100;

	virtual UAttributeSet* CreateAttributeSet() override;
};
