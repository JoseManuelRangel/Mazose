// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Deadzone.h"

#include "Kismet/GameplayStatics.h"

#include "Player/BallPlayerController.h"

#include "Widgets/MZ_LevelWidget.h"

#include "MZ_GameMode.h"


AMZ_Deadzone::AMZ_Deadzone()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = true;

	/* Creo la caja de colisión y lo seteo como root component. */
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);

	/* Vinculo el evento de colisión. */
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Deadzone::OnOverlapBegin);
}

void AMZ_Deadzone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == (UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (!PC)
		{
			return;
		}

		ABallPlayerController* Controller = Cast<ABallPlayerController>(PC);

		if (!Controller)
		{
			return;
		}

		UMZ_LevelWidget* Widget = Cast<UMZ_LevelWidget>(Controller->CurrentLevelWidget);

		if (!Widget)
		{
			return;
		}

		Widget->OnSettingIsAliveToFalse.Broadcast();

		AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld());

		if (!GM)
		{
			return;
		}

		AMZ_GameMode* GameMode = Cast<AMZ_GameMode>(GM);

		if (!GameMode)
		{
			return;
		}

		GameMode->OnStopSound.Broadcast();
	}
}

