// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSelectionComponent, Log, All);

// Sets default values for this component's properties
USelectionComponent::USelectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bDebugEnabledByDefault = false;
	TraceLength = 10000;
	BoxSize = 5.0f;
}


// Called when the game starts
void USelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	bDebugEnabled = bDebugEnabledByDefault;
	
	GetPlayerController()->SetShowMouseCursor(true);
	SetupInput();
}

// Called every frame
void USelectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Hover = GetHoverData();
	SetMouseCursor(Hover.IsValid() ? FName("Attackable") : FName("Default"));
}

void USelectionComponent::OnDoSelection()
{
	if(Hover.AbilitySystemComponent == Selection)
		return;
	
	const TObjectPtr<UAbilitySystemComponent> OldSelection = Selection;
	Selection = Hover.AbilitySystemComponent;

	OnSelectionChanged(OldSelection, Selection);
}

USelectionComponent::HoverData USelectionComponent::GetHoverData() const
{
	HoverData Hovered = { nullptr, nullptr };

	TArray<FHitResult> Hits = TraceFromScreen();

	const FHitResult* BestHit = nullptr;
	for (FHitResult Hit : Hits)
	{
		if(IsSelectionCandidate(Hit))
		{
			if(BestHit == nullptr)
			{
				BestHit = &Hit;
				DrawDebugBox(Hit.Location, FColor::Green);
			}
			else
			{
				DrawDebugBox(Hit.Location, FColor::Blue);			
			}			
		}
		else
		{
			DrawDebugBox(Hit.Location, FColor::Red);
		}
	}

	if(BestHit != nullptr)
	{
		Hovered.Actor = BestHit->GetActor();
		
		const auto SystemInterface = Cast<IAbilitySystemInterface>(Hovered.Actor);		
		Hovered.AbilitySystemComponent = SystemInterface->GetAbilitySystemComponent();
	}

	return Hovered;
}

bool USelectionComponent::IsSelectionCandidate(const FHitResult& Hit) const
{
	if(Hit.GetActor() == GetPlayerController()->GetPawn())
		return false;
	
	return Hit.GetActor()->Implements<UAbilitySystemInterface>();
}

TArray<FHitResult> USelectionComponent::TraceFromScreen() const
{
	float MouseX, MouseY;
	GetPlayerController()->GetMousePosition(MouseX, MouseY);

	FVector WorldPosition, WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(GetPlayerController(), FVector2D(MouseX, MouseY), WorldPosition, WorldDirection);

	const FVector TraceStart = WorldPosition;
	const FVector TraceEnd = WorldPosition + WorldDirection * TraceLength;

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
	CollisionQueryParams.AddIgnoredActor(GetPlayerController()->GetPawn());
	
	TArray<FHitResult> Hits;
	GetWorld()->LineTraceMultiByObjectType(Hits, TraceStart, TraceEnd, FCollisionObjectQueryParams::DefaultObjectQueryParam, CollisionQueryParams);

	if(bDebugEnabled)
	{
		// Draw the full line
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White);
	}

	return Hits;
}

void USelectionComponent::DrawDebugBox(const FVector Location, const FColor Color) const
{
	if(!bDebugEnabled)
		return;
	
	DrawDebugSolidBox(GetWorld(), Location, FVector::One() * BoxSize, Color);
}

void USelectionComponent::OnSelectionChanged(TObjectPtr<UAbilitySystemComponent> OldSelection, TObjectPtr<UAbilitySystemComponent> NewSelection)
{
	EnsureEffectExists();
	Effect->SetTarget(NewSelection == nullptr ? nullptr : NewSelection->GetOwner());
}

void USelectionComponent::SetupInput()
{
	const auto InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	check(InputComponent);

	FInputActionBinding Binding = InputComponent->BindAction(FName("Select"), EInputEvent::IE_Released, this, &USelectionComponent::OnDoSelection);
	if(!Binding.IsPaired())
	{
		UE_LOG(LogSelectionComponent, Warning, TEXT("Selection binding failed"));
	}
}

void USelectionComponent::SetMouseCursor(const FName CursorName)
{
	UWidgetBlueprintLibrary::SetHardwareCursor(this, EMouseCursor::Default, CursorName, FVector2D::ZeroVector);
}

void USelectionComponent::EnsureEffectExists()
{
	if(!IsValid(Effect))
	{
		Effect = GetWorld()->SpawnActor<ASelectionEffect>(SelectionEffectType);
	}
}
