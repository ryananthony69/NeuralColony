#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColonyZoneVisualActor.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class NEURALCOLONY_API AColonyZoneVisualActor : public AActor
{
	GENERATED_BODY()

public:
	AColonyZoneVisualActor();
	void InitializeZone(const FString& ZoneName, const FVector& ZoneSize, const FLinearColor& ZoneColor);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Floor;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Border;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* Label;
};
