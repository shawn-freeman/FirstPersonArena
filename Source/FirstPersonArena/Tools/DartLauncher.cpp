// Fill out your copyright notice in the Description page of Project Settings.


#include "DartLauncher.h"
#include "FirstPersonArena/AdventureCharacter.h"

void ADartLauncher::Use()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Using the dart launcher!"));
}

// Called every frame
void ADartLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADartLauncher::BindInputAction(const UInputAction* InputToBind)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("=== DARTLAUNCHER: BindInputAction() called ==="));

	if (!InputToBind)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DARTLAUNCHER: InputToBind is NULL!"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DARTLAUNCHER: InputToBind is valid"));

	if (!OwningCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DARTLAUNCHER: OwningCharacter is NULL!"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DARTLAUNCHER: OwningCharacter is valid"));

	APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController());
	if (!PlayerController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DARTLAUNCHER: Failed to get PlayerController from OwningCharacter!"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DARTLAUNCHER: PlayerController found"));

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	if (!EnhancedInputComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DARTLAUNCHER: Failed to cast to EnhancedInputComponent!"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("DARTLAUNCHER: EnhancedInputComponent found"));

	// Bind the Use action
	EnhancedInputComponent->BindAction(InputToBind, ETriggerEvent::Triggered, this, &ADartLauncher::Use);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DARTLAUNCHER: UseAction successfully bound to Use() method!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("=== DARTLAUNCHER: BindInputAction() complete ==="));
}
