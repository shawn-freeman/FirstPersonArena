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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ADartLauncher::BindInputAction"));
	if (APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController())) {
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			//Fire
			EnhancedInputComponent->BindAction(InputToBind, ETriggerEvent::Triggered, this, &ADartLauncher::Use);
		}
	}
}
