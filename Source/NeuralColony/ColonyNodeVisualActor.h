#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColonySimulationTypes.h"
#include "ColonyNodeVisualActor.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeVisualSelected, const FGuid&, NodeId);

UCLASS()
class NEURALCOLONY_API AColonyNodeVisualActor : public AActor
{
	GENERATED_BODY()

public:
	AColonyNodeVisualActor();

	void ApplyNodeRecord(const FNodeRecord& NodeRecord);
	FGuid GetNodeId() const { return NodeId; }
	FString GetNodeName() const { return NodeName; }
	void SetConversationText(const FString& ConversationText);
	void SetSelected(bool bInSelected);

	UPROPERTY(BlueprintAssignable, Category = "NeuralColony|Presentation")
	FOnNodeVisualSelected OnNodeSelected;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleClicked(AActor* TouchedActor, FKey ButtonPressed);

	FLinearColor GetStateColor(ENodeLifecycleState State) const;
	FString GetStateLabel(ENodeLifecycleState State) const;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* Label;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* Bubble;

	FGuid NodeId;
	FString NodeName;
};
