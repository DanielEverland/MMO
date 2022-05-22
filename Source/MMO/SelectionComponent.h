// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectionEffect.h"
#include "Components/ActorComponent.h"
#include "SelectionComponent.generated.h"

class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMO_API USelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USelectionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsSelectionCandidate(const FHitResult& Hit) const;

protected:
	struct HoverData
	{
		AActor* Actor;
		UAbilitySystemComponent* AbilitySystemComponent;

		FORCEINLINE bool IsValid() const { return Actor != nullptr && AbilitySystemComponent != nullptr; }
	};
	
	virtual void BeginPlay() override;
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetOwner()); }
	HoverData GetHoverData() const;
	TArray<FHitResult> TraceFromScreen() const;
	void DrawDebugBox(FVector Location, FColor Color) const;

	virtual void OnSelectionChanged(TObjectPtr<UAbilitySystemComponent> OldSelection, TObjectPtr<UAbilitySystemComponent> NewSelection);

	UFUNCTION()
	void OnDoSelection();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASelectionEffect> SelectionEffectType;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float TraceLength;

	UPROPERTY(EditAnywhere, Category = "Advanced|Debugging", meta = (AllowPrivateAccess = "true"))
	bool bDebugEnabledByDefault;
	
	UPROPERTY(EditAnywhere, Category = "Advanced|Debugging", meta = (AllowPrivateAccess = "true"))
	float BoxSize;
	
	UPROPERTY()
	bool bDebugEnabled = true;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> Selection;

	UPROPERTY()
	TObjectPtr<ASelectionEffect> Effect;

	HoverData Hover;
	
	void SetupInput();
	void SetMouseCursor(FName CursorName);
	void EnsureEffectExists();
};
