// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionEffect.h"


// Sets default values
ASelectionEffect::ASelectionEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASelectionEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASelectionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Target == nullptr)
		return;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Target);
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, Target->GetActorLocation(), Target->GetActorLocation() + FVector::DownVector * 10000, ECC_WorldStatic, QueryParams);

	SetActorLocation(Hit.Location);
	SetActorRotation(Target->GetActorForwardVector().ToOrientationRotator());
}

void ASelectionEffect::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;
	SetVisibility(Target != nullptr);
}

