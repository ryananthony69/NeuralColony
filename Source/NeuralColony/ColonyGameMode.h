#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ColonyGameMode.generated.h"

UCLASS()
class NEURALCOLONY_API AColonyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AColonyGameMode();
	virtual void BeginPlay() override;
};
