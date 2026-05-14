// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Changezone.h"

#include "Kismet/GameplayStatics.h"

#include "Player/BallPlayerController.h"

#include "Widgets/MZ_LevelWidget.h"


AMZ_Changezone::AMZ_Changezone()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = true;

	/* Creo el componente de la caja de colisión y lo establezco como root en el actor. */
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionComponent);

	/* Le doy la funcionalidad al evento de overlap. */
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Changezone::OnOverlapBegin);
}

void AMZ_Changezone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

		if (Controller && Controller->CurrentLevelWidget)
		{
			/* Le hacemos el casteo al widget del nivel. */
			UMZ_LevelWidget* W = Cast<UMZ_LevelWidget>(Controller->CurrentLevelWidget);

			/* Early return del level widget. */
			if (!W)
			{
				return;
			}

			/* Si el casteo es válido hacemos el broadcast del event dispatcher. */
			W->OnUpdateBallIndicatorPosition.Broadcast();
		}
	}
}

