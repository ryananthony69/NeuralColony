#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColonySimulationTypes.h"
#include "ColonyProjectSiteActor.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class NEURALCOLONY_API AColonyProjectSiteActor : public AActor
{
	GENERATED_BODY()

public:
	AColonyProjectSiteActor();
	void ApplyProjectRecord(const FProjectRecord& ProjectRecord);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SiteMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProgressMesh;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* Label;
};
