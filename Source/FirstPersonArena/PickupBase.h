// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h" //You�ll add a sphere component to the pickup to detect collisions between player and pickup.
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "AdventureCharacter.h" //Add a reference to your first-person character class so you can check for overlaps between the pickup and characters of this class.
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class UItemDefinition;

UCLASS(BlueprintType, Blueprintable)
class FIRSTPERSONARENA_API APickupBase : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//In the Category text, the vertical bar(| ) creates a nested subsection.So in this example, Unreal Engine creates a Pickup section with a subsection named Item Table in the asset�s Details panel.
	// The ID of this pickup in the associated data table.
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	FName PickupItemID;

	// Data table that contains this pickup.
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Table")
	TSoftObjectPtr<UDataTable> PickupDataTable;

	// Data asset associated with this item.
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Reference Item")
	TObjectPtr<UItemDefinition> ReferenceItem;

	// The mesh component to represent this pickup in the world.
	UPROPERTY(VisibleDefaultsOnly, Category = "Pickup | Mesh")
	TObjectPtr<UStaticMeshComponent> PickupMeshComponent;

	// Sphere Component that defines the collision radius of this pickup for interaction purposes.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Components")
	TObjectPtr<USphereComponent> SphereComponent;

	// Code for when something overlaps the SphereComponent. 
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Whether this pickup should respawn after being picked up.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Respawn")
	bool bShouldRespawn;

	// The time in seconds to wait before respawning this pickup.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Respawn")
	float RespawnTime = 4.0f;

	// Timer handle to distinguish the respawn timer.
	FTimerHandle RespawnTimerHandle;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializePickup();
};
