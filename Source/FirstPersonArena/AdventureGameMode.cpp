// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGameMode.h"

void AAdventureGameMode::StartPlay() {
	Super::StartPlay();

	//global engine pointer is valid
	check(GEngine != nullptr);

	//On screen debug message
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is Adventure Game Mode!"));

	//Engine logs
	UE_LOG(LogTemp, Warning, TEXT("This is a warning message!"));
}