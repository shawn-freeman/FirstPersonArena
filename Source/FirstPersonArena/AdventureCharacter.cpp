// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureCharacter.h"
#include "InventoryComponent.h"
#include "EquippableToolDefinition.h"
#include "EquippableToolBase.h"

// Sets default values
AAdventureCharacter::AAdventureCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it
	PrimaryActorTick.bCanEverTick = true;

	// Create a first-person camera component
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FirstPersonCameraComponent != nullptr);

	// Create a first-person mesh component for the owning player
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FirstPersonMeshComponent != nullptr);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	check(InventoryComponent != nullptr);

	// Attach the first-person mesh to the skeletal mesh
	FirstPersonMeshComponent->SetupAttachment(GetMesh());

	// The first-person mesh is included in First Person rendering (use FirstPersonFieldofView and FirstPersonScale on this mesh) 
	FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

	// Only the owning player sees the first-person mesh
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);

	// The owning player doesn't see the regular (third-person) body mesh, but it casts a shadow
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	// Set the first-person mesh to not collide with other objects
	FirstPersonMeshComponent->SetCollisionProfileName(FName("NoCollision"));

	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));

	// Position the camera slightly above the eyes and rotate it to behind the player's head
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Enable first-person rendering on the camera and set default FOV and scale values
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = FirstPersonFieldOfView;
	FirstPersonCameraComponent->FirstPersonScale = FirstPersonViewScale;
}

// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Only the owning player sees the first person mesh
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);

	// Set the animations on the first person mesh.
	FirstPersonMeshComponent->SetAnimInstanceClass(FirstPersonDefaultAnim->GeneratedClass);

	// The owning player doesn't see the regular (third-person) body mesh
	GetMesh()->SetOwnerNoSee(true);

	// Position the camera slightly above the eyes.
	FirstPersonCameraComponent->SetRelativeLocation(FVector(2.8f, 5.9f, 0.0f));

	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the enhanced input local player subsystem and add a new input mapping context to it
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FirstPersonContext, 0);
		}
	}

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AdventureCharacter."));
}

// Called every frame
void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdventureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Check the UInputComponent passed to this function and cast it to an UEnhancedInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement Actions
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Move);

		// Bind Look Actions
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Look);

		// Bind Jump Actions
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}

}

void AAdventureCharacter::Move(const FInputActionValue& Value)
{
	// 2D Vector of movement values returned from the input action
	const FVector2D MovementValue = Value.Get<FVector2D>();

	if (Controller) {
		const FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementValue.X);

		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementValue.Y);
	}
}

void AAdventureCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

bool AAdventureCharacter::IsToolAlreadyOwned(UEquippableToolDefinition* ToolDefinition)
{
	for (UEquippableToolDefinition* InventoryItem: InventoryComponent->ToolInventory)
	{
		if (ToolDefinition->ID == InventoryItem->ID) {
			return true;
		}
	}

	return false;
}

void AAdventureCharacter::AttachTool(UEquippableToolDefinition* ToolDefinition)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("=== ATTACH_TOOL: Starting AttachTool() ==="));

	// Only equip this tool if it isn't already owned
	if (!IsToolAlreadyOwned(ToolDefinition))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: Tool not already owned, spawning..."));

		// Spawn a new instance of the tool to equip
		AEquippableToolBase* ToolToEquip = GetWorld()->SpawnActor<AEquippableToolBase>(ToolDefinition->ToolAsset, this->GetActorTransform());

		if (ToolToEquip && InventoryComponent)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: Tool spawned successfully"));

			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

			// Attach the tool to this character, and then the right hand of their first-person mesh
			ToolToEquip->AttachToActor(this, AttachmentRules);
			ToolToEquip->AttachToComponent(FirstPersonMeshComponent, AttachmentRules, FName(TEXT("HandGrip_R")));
			ToolToEquip->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("HandGrip_R")));

			ToolToEquip->OwningCharacter = this;

			InventoryComponent->ToolInventory.Add(ToolDefinition);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: Tool attached to character and added to inventory"));

			//Add the item's animations to the character
			FirstPersonMeshComponent->SetAnimInstanceClass(ToolToEquip->FirstPersonToolAnim->GeneratedClass);
			GetMesh()->SetAnimInstanceClass(ToolToEquip->ThirdPersonToolAnim->GeneratedClass);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: Animations set on character meshes"));

			EquippedTool = ToolToEquip;

			//Get the player controller for this character
			if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: PlayerController found"));

				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: EnhancedInputSubsystem found"));
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("INPUT: About to add tool mapping context..."));

					// Only add the tool's mapping context if it's valid
					if (ToolToEquip->ToolMappingContext)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("INPUT: Tool has ToolMappingContext set! Adding with priority 1..."));
						//The priority here is important. The character's main InputMapping is '0' so this should have a higher priority when it's equipped
						Subsystem->AddMappingContext(ToolToEquip->ToolMappingContext, 1);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("INPUT: ToolMappingContext added successfully!"));
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("INPUT: Tool has NO ToolMappingContext set!"));
					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ATTACH_TOOL: Failed to get EnhancedInputSubsystem!"));
				}

				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: Calling BindInputAction..."));
				ToolToEquip->BindInputAction(UseAction);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ATTACH_TOOL: BindInputAction complete"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ATTACH_TOOL: Failed to get PlayerController!"));
			}

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("=== ATTACH_TOOL: Complete ==="));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ATTACH_TOOL: Failed to spawn tool or InventoryComponent is null!"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ATTACH_TOOL: Tool already owned, skipping..."));
	}
}

void AAdventureCharacter::GiveItem(UItemDefinition* ItemDefinition)
{
	switch (ItemDefinition->ItemType) 
	{
	case EItemType::Consumable :
	{
		//Not Implemented
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Attempting to give the player a consumable item."));
		break;
	}

	case EItemType::Tool:
	{
		// If the item is a tool, attempt to cast and attach it to the character
		UEquippableToolDefinition* ToolDefinition = Cast<UEquippableToolDefinition>(ItemDefinition);
		if (ToolDefinition != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Give Tool Item to player"));
			AttachTool(ToolDefinition);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cast to tool failed!"));
		}
		break;
	}
		
	default :
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Item attempted to give player does not match any ENUM type."));
		break;
	}
}
