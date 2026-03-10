#include "ColonyProjectSiteActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

AColonyProjectSiteActor::AColonyProjectSiteActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SitePad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SitePad"));
	SitePad->SetupAttachment(Root);
	SitePad->SetWorldScale3D(FVector(5.5f, 5.5f, 0.25f));

	ProgressBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProgressBar"));
	ProgressBar->SetupAttachment(Root);
	ProgressBar->SetRelativeLocation(FVector(-420.0f, -350.0f, 30.0f));
	ProgressBar->SetWorldScale3D(FVector(0.1f, 0.55f, 0.18f));

	BuildMass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildMass"));
	BuildMass->SetupAttachment(Root);
	BuildMass->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	BuildMass->SetWorldScale3D(FVector(1.2f, 1.2f, 0.6f));

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Label->SetWorldSize(34.0f);
	Label->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		SitePad->SetStaticMesh(CubeMesh.Object);
		ProgressBar->SetStaticMesh(CubeMesh.Object);
		BuildMass->SetStaticMesh(CubeMesh.Object);
	}
}

void AColonyProjectSiteActor::ApplyProjectRecord(const FProjectRecord& ProjectRecord)
{
	const float Progress = FMath::Clamp(ProjectRecord.BuildProgress, 0.0f, 1.0f);
	ProgressBar->SetWorldScale3D(FVector(FMath::Lerp(0.1f, 8.3f, Progress), 0.55f, 0.18f));
	ProgressBar->SetRelativeLocation(FVector(FMath::Lerp(-420.0f, -30.0f, Progress), -350.0f, 30.0f));

	const float BuildHeight = FMath::Lerp(0.35f, 2.6f, Progress);
	BuildMass->SetWorldScale3D(FVector(1.25f, 1.25f, BuildHeight));
	BuildMass->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f + (BuildHeight * 38.0f)));

	const FString StageLabel = ProjectRecord.CurrentStage.IsEmpty()
		? StaticEnum<EProjectState>()->GetNameStringByValue(static_cast<int64>(ProjectRecord.State))
		: ProjectRecord.CurrentStage;

	Label->SetText(FText::FromString(FString::Printf(TEXT("%s\nStage: %s\nBuild: %.0f%%"), *ProjectRecord.Name, *StageLabel, Progress * 100.0f)));
}
