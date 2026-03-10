#include "ColonyProjectSiteActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

AColonyProjectSiteActor::AColonyProjectSiteActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SiteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SiteMesh"));
	SiteMesh->SetupAttachment(Root);
	SiteMesh->SetWorldScale3D(FVector(4.0f, 4.0f, 0.2f));

	ProgressMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProgressMesh"));
	ProgressMesh->SetupAttachment(Root);
	ProgressMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	ProgressMesh->SetWorldScale3D(FVector(0.2f, 3.8f, 0.2f));

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Label->SetWorldSize(30.0f);
	Label->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		SiteMesh->SetStaticMesh(CubeMesh.Object);
		ProgressMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void AColonyProjectSiteActor::ApplyProjectRecord(const FProjectRecord& ProjectRecord)
{
	const float ClampedProgress = FMath::Clamp(ProjectRecord.BuildProgress, 0.0f, 1.0f);
	ProgressMesh->SetWorldScale3D(FVector(FMath::Max(ClampedProgress * 3.8f, 0.05f), 0.4f, 0.2f));
	ProgressMesh->SetRelativeLocation(FVector(-350.0f + (ClampedProgress * 350.0f), 0.0f, 30.0f));

	const FString StateLabel = StaticEnum<EProjectState>()->GetNameStringByValue(static_cast<int64>(ProjectRecord.State));
	Label->SetText(FText::FromString(FString::Printf(TEXT("%s\n%s %.0f%%"), *ProjectRecord.Name, *StateLabel, ClampedProgress * 100.0f)));
}
