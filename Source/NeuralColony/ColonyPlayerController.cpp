#include "ColonyPlayerController.h"

#include "ColonyHUDWidget.h"

void AColonyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	SetInputMode(FInputModeGameAndUI());
}

void AColonyPlayerController::SetHUDWidget(UColonyHUDWidget* InWidget)
{
	HUDWidget = InWidget;
}

void AColonyPlayerController::SetSelectedNodeId(const FGuid& InSelectedNodeId)
{
	if (HUDWidget)
	{
		HUDWidget->SetSelectedNodeId(InSelectedNodeId);
	}
}
