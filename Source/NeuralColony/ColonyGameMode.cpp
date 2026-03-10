#include "ColonyGameMode.h"

#include "ColonyCameraPawn.h"
#include "ColonyPlayerController.h"
#include "ColonyPresentationManager.h"

AColonyGameMode::AColonyGameMode()
{
	DefaultPawnClass = AColonyCameraPawn::StaticClass();
	PlayerControllerClass = AColonyPlayerController::StaticClass();
}

void AColonyGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->SpawnActor<AColonyPresentationManager>(FVector::ZeroVector, FRotator::ZeroRotator);
}
