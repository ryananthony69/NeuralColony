#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColonySimulationTypes.h"
#include "ColonyPresentationManager.generated.h"

class UColonySimulationSubsystem;
class AColonyNodeVisualActor;
class AColonyProjectSiteActor;
class UColonyHUDWidget;

UCLASS()
class NEURALCOLONY_API AColonyPresentationManager : public AActor
{
	GENERATED_BODY()

public:
	AColonyPresentationManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void EnsureNodeActors();
	void SyncNodeActors();
	void SyncProjectActor();
	void ApplyConversationBubbles();

	UFUNCTION()
	void HandleNodeSelected(const FGuid& NodeId);

private:
	UPROPERTY()
	UColonySimulationSubsystem* SimulationSubsystem;

	UPROPERTY()
	TMap<FGuid, AColonyNodeVisualActor*> NodeActors;

	UPROPERTY()
	AColonyProjectSiteActor* ProjectActor;

	UPROPERTY()
	UColonyHUDWidget* HUDWidget;

	FGuid SelectedNodeId;
	FString LastConversationAlert;
};
