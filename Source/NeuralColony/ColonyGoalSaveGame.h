#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ColonyGoalSaveGame.generated.h"

UCLASS()
class NEURALCOLONY_API UColonyGoalSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NeuralColony|Goals")
	FString PlayerGoal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NeuralColony|Goals")
	FString NeuralNetworkGoal;
};
