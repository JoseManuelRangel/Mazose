// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Coin.h"

#include "NiagaraComponent.h"

#include "Components/AudioComponent.h"

#include "Kismet/GameplayStatics.h"

#include "MZ_GameMode.h"

#include "Player/BallPlayerController.h"

#include "Widgets/MZ_LevelWidget.h"

#include "Sound/SoundBase.h"

#include "NiagaraSystem.h"

AMZ_Coin::AMZ_Coin()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = false;

	/* Creo el mesh y lo establezco como raíz. */
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Coin Mesh"));
	SetRootComponent(CoinMesh);

	/* Creo la colisión y la atacho al mesh de la moneda. */
	CoinCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Coin Collision"));
	CoinCollision->SetupAttachment(CoinMesh);
	/* Le añado el evento de overlap para la esfera de colisión. */
	CoinCollision->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Coin::OnOverlapBegin);

	/* Creo el componente de partículas para la moneda y la atacho al mesh de la moneda.*/
	CoinParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Coin Particles"));
	CoinParticles->SetupAttachment(CoinMesh);

	/* Creo el componente de audio para la moneda y la atacho al mesh de la moneda. */
	CoinAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Coin Sound"));
	CoinAudio->SetupAttachment(CoinMesh);

	/* No activo de principio el sonido. */
	CoinAudio->bAutoActivate = false;
}

void AMZ_Coin::BeginPlay()
{
	Super::BeginPlay();

	/* Compruebo la validez del componente y del asset de las partículas. */
	if (CoinParticles && CoinParticlesAsset)
	{
		/* Seteo el asset y lo activo.*/
		CoinParticles->SetAsset(CoinParticlesAsset);
		CoinParticles->Activate();
	}

	/* Compruebo la validez del componente y del asset del sonido. */
	if (CoinAudio && CoinAudioAsset)
	{
		/* Seteo el asset. */
		CoinAudio->SetSound(CoinAudioAsset);
	}
}

void AMZ_Coin::CollectingCoin()
{
	/* Early return para comprobar la validez del sonido de la moneda. */
	if (!CoinAudio)
	{
		return;
	}

	/* Ponemos el sonido de recogida de moneda en la localización del actor. */
	UGameplayStatics::PlaySoundAtLocation(this, CoinAudioAsset, GetActorLocation());

	/* Desactivamos las partículas de la moneda. */
	CoinParticles->Deactivate();

	/* Destruye la moneda. */
	this->Destroy();
}

void AMZ_Coin::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si no está la moneda recolectada y el otro actor es el pawn... */
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag(FName(TEXT("Pawn"))))
		{
			if (!bWasCollected && (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
			{
				/* Notifico que ahora sí esta recolectada. */
				bWasCollected = true;

				/* Casteo al controller personalizado. */
				ABallPlayerController* PC = Cast<ABallPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

				if (PC && PC->CurrentLevelWidget)
				{
					/* Le hacemos el casteo al widget del nivel. */
					UMZ_LevelWidget* W = Cast<UMZ_LevelWidget>(PC->CurrentLevelWidget);

					/* Early return del level widget. */
					if (!W)
					{
						return;
					}

					/* Llamamos a la función local de moneda recolectada. */
					CollectingCoin();

					/* Si el casteo es válido hacemos el broadcast del event dispatcher. */
					W->AddingCoins();
				}
			}
		}
	}
}

