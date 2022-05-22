// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectionEffect.generated.h"

UCLASS()
class MMO_API ASelectionEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASelectionEffect();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTarget(AActor* NewTarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetVisibility(bool IsVisible);

private:
	UPROPERTY()
	TObjectPtr<AActor> Target;
};
