#include "ColonyZoneVisualActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

AColonyZoneVisualActor::AColonyZoneVisualActor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(Root);
	Floor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Border = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Border"));
	Border->SetupAttachment(Root);
	Border->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Border->SetRelativeLocation(FVector(0.0f, 0.0f, -8.0f));

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Root);
	Label->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Label->SetWorldSize(42.0f);
	Label->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (PlaneMesh.Succeeded())
	{
		Floor->SetStaticMesh(PlaneMesh.Object);
	}
	if (CubeMesh.Succeeded())
	{
		Border->SetStaticMesh(CubeMesh.Object);
	}
}

void AColonyZoneVisualActor::InitializeZone(const FString& ZoneName, const FVector& ZoneSize, const FLinearColor& ZoneColor)
{
	Floor->SetWorldScale3D(FVector(ZoneSize.X / 100.0f, ZoneSize.Y / 100.0f, 1.0f));
	Floor->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(ZoneColor.R, ZoneColor.G, ZoneColor.B));

	Border->SetWorldScale3D(FVector(ZoneSize.X / 100.0f, ZoneSize.Y / 100.0f, 0.06f));
	Border->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(ZoneColor.R * 0.45f, ZoneColor.G * 0.45f, ZoneColor.B * 0.45f));

	Label->SetText(FText::FromString(ZoneName));
	Label->SetTextRenderColor(ZoneColor.ToFColor(true));
}
