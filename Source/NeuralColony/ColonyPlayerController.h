#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ColonyPlayerController.generated.h"

class UColonyHUDWidget;

UCLASS()
class NEURALCOLONY_API AColonyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetHUDWidget(UColonyHUDWidget* InWidget);
	void SetSelectedNodeId(const FGuid& InSelectedNodeId);

private:
	UPROPERTY()
	UColonyHUDWidget* HUDWidget;
};
