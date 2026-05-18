// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Platform.h"

#include "Kismet/GameplayStatics.h"

#include "MZ_GameMode.h"

AMZ_Platform::AMZ_Platform()
{
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(BaseMesh);

	BorderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Border Mesh"));
	BorderMesh->SetupAttachment(BaseMesh);
}

void AMZ_Platform::BeginPlay()
{
	Super::BeginPlay();

	AMZ_GameMode* GM = Cast<AMZ_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GM)
	{
		return;
	}

	GM->RegisterSpawnPlatform(this);
}


