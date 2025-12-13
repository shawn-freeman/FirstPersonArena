// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" //Required to have your struct inherit from FTableRowBase
#include "ItemData.generated.h" //Required by Unreal Header Tool. Make sure this include statement comes last for your code to compile properly.

class UItemDefinition;

// Defines the type of the item.
UENUM()
enum class EItemType : uint8
{
	Tool UMETA(DisplayName = "Tool"),
	Consumable UMETA(DisplayName = "Consumable")
};

USTRUCT()
struct FItemText
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType itemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemText ItemText;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	UItemDefinition* ItemBase;
};