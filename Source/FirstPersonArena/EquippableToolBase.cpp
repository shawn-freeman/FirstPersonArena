// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableToolBase.h"

// Sets default values
AEquippableToolBase::AEquippableToolBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ToolMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ToolMesh"));
	check(ToolMeshComponent != nullptr);
}

// Called when the game starts or when spawned
void AEquippableToolBase::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("=== TOOL: BeginPlay() called ==="));

	if (ToolMappingContext)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("TOOL: ToolMappingContext is set"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TOOL: ToolMappingContext is NULL!"));
	}

	if (OwningCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("TOOL: OwningCharacter is set"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TOOL: OwningCharacter is NULL!"));
	}
}

// Called every frame
void AEquippableToolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEquippableToolBase::Use()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("TOOL: Use() called"));
}

void AEquippableToolBase::BindInputAction(const UInputAction* ActionToBind)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("=== TOOL: BindInputAction() called ==="));

	if (ActionToBind)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("TOOL: ActionToBind is valid"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("TOOL: ActionToBind is NULL!"));
	}

	// TODO: Implement actual input binding here
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TOOL: BindInputAction is currently empty (not implemented)"));
}

