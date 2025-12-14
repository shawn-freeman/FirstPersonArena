// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDefinition.h"
#include "EquippableToolDefinition.generated.h"

class AEquippableToolBase; //The Actor representing your tools in-game. 
class UInputMappingContext;

UCLASS(BlueprintType, Blueprintable)
class FIRSTPERSONARENA_API UEquippableToolDefinition : public UItemDefinition
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEquippableToolBase> ToolAsset;

	UFUNCTION()
	virtual UEquippableToolDefinition* CreateItemCopy() const override;
};
