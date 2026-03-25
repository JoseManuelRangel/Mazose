// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_Stage.h"
#include "Kismet/GameplayStatics.h"

AMZ_Stage::AMZ_Stage()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = true;

	/* Creo la base del stage y lo seteo como Root. */
	StageBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage Base"));
	SetRootComponent(StageBase);

	/* Creo los bordes del stage y los atacho a la base. */
	StageBorders = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage Borders"));
	StageBorders->SetupAttachment(StageBase);

	/* Creo la colisión del stage, la atacho a la base y le doy funcionalidad a la caja de colisión al chocar con el Pawn. */
	StageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Stage Collision"));
	StageCollision->SetupAttachment(StageBase);
	StageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMZ_Stage::OnOverlapBegin);

	/* Creo el sonido del stage al ser pisado. */
	StageSound = CreateDefaultSubobject<USoundBase>(TEXT("Stage Sound"));
}

void AMZ_Stage::BeginPlay()
{
	Super::BeginPlay();

	/* Al empezar el juego seteo el material elegido para el stage en sus correspondientes sitios */
	//StageBase->SetMaterial(0, StageMaterial);
	//StageBorders->SetMaterial(0, StageMaterial);	
}

void AMZ_Stage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMZ_Stage::PlayActivatingSound()
{
	/* Si el sonido es nulo, hago un early return. */
	if (!StageSound)
	{
		return;
	}

	/* Reproduce el sonido en la ubicación del actor. */
	UGameplayStatics::PlaySoundAtLocation(this, StageSound, GetActorLocation());
}

void AMZ_Stage::HandleScaleDecreaseProgress()
{
	/* Cargo la escala inicial del componente base del stage. */
	InitialScale = StageBase->GetRelativeScale3D();

	/* Seteo el tiempo transcurrido del timer a 0.0f. */
	ElapsedTime = 0.0f;

	/* Delay que quiero darle antes de que empiece el Timer. */
	float DelayAmount = 1.0f;

	/* Creación del Timer. */
	GetWorldTimerManager().SetTimer(
		ScaleTimerHandle, 
		this, 
		&AMZ_Stage::UpdateScale, 
		0.01f, 
		true,
		DelayAmount);
}

void AMZ_Stage::UpdateScale()
{
	/* Sumamos 0.01 segundos al tiempo transcurrido. */
	ElapsedTime += 0.01f;

	/* Clampeamos el valor del tiempo de animación entre el tiempo transcurrido. */
	float Alpha = FMath::Clamp(ElapsedTime / AnimationTime, 0.0f, 1.0f);

	/* Guardamos la nueva escala con uan interpolación lineal entre la inicial y la final. */
	FVector NewScale = FMath::Lerp(InitialScale, FinalScale, Alpha);
	/* Seteamos la escala nueva. */
	StageBase->SetRelativeScale3D(NewScale);

	/* Si el tiempo transcurrido es de más de 1 segundo limpio el timer y destruyo la plataforma. */
	if (Alpha >= 1.0f)
	{
		GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
		this->Destroy();
	}
}

void AMZ_Stage::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si el otro actor es válido y no es ese actor... */
	if (OtherActor && (OtherActor != this))
	{
		/* Si el otro actor tiene la tag "Pawn"... */
		if (OtherActor->ActorHasTag(FName(TEXT("Pawn"))) && !bIsActivated)
		{
			/* Seteo el material al de haber colisionado, le doy play al sonido de colisión, notifico que está activada e inicio el Timer. */
			StageBase->SetMaterial(0, StageCollisionMaterial);
			PlayActivatingSound();
			bIsActivated = true;
			HandleScaleDecreaseProgress();
		}
	}
}



