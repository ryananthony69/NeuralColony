#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColonySimulationTypes.h"
#include "ColonyPresentationManager.generated.h"

class UColonySimulationSubsystem;
class AColonyNodeVisualActor;
class AColonyProjectSiteActor;
class AColonyZoneVisualActor;
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
	void BuildColonyLayout();
	void EnsureNodeActors();
	void SyncNodeActors();
	void SyncProjectActor();
	void ApplyConversationBehavior();
	FVector ResolveTargetLocationForNode(const FNodeRecord& Node) const;
	FVector GetSlotPosition(const FName& ZoneName, int32 SlotIndex) const;

	UFUNCTION()
	void HandleNodeSelected(const FGuid& NodeId);

private:
	UPROPERTY()
	UColonySimulationSubsystem* SimulationSubsystem;

	UPROPERTY()
	TMap<FGuid, AColonyNodeVisualActor*> NodeActors;

	UPROPERTY()
	TArray<AColonyZoneVisualActor*> ZoneActors;

	UPROPERTY()
	AColonyProjectSiteActor* ProjectActor;

	UPROPERTY()
	UColonyHUDWidget* HUDWidget;

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Layout")
	FVector NurseryCenter = FVector(-1400.0f, -820.0f, 80.0f);

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Layout")
	FVector ArchitectCoreCenter = FVector(-1200.0f, 650.0f, 80.0f);

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Layout")
	FVector WorkshopCenter = FVector(220.0f, 620.0f, 80.0f);

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Layout")
	FVector VerificationCenter = FVector(1350.0f, 640.0f, 80.0f);

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Layout")
	FVector RepairCenter = FVector(1300.0f, -720.0f, 80.0f);

	UPROPERTY(EditAnywhere, Category = "NeuralColony|Layout")
	FVector ProjectCenter = FVector(130.0f, -710.0f, 65.0f);

	FGuid SelectedNodeId;
	FString LastConversationAlert;
};
