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
	UStaticMeshComponent* SitePad;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProgressBar;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BuildMass;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* Label;
};
