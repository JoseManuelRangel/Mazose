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

	/* Creo el componente de escena para colocar la fresa que aparezca en el stage. */
	StrawberryPosition = CreateDefaultSubobject<USceneComponent>(TEXT("StrawberryPosition"));
	StrawberryPosition->SetupAttachment(StageBase);

	/* Busco el material por defecto en las rutas del proyecto para cargarlo como objeto estático. */
	DefaultMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, 
		TEXT("/Script/Engine.Material'/Game/Materials/M_Black.M_Black'")));

	/* Busco el material para la colisión en las rutas del proyecto para cargarlo como objeto estático. */
	CollisionMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,
		TEXT("/Script/Engine.Material'/Game/Materials/M_Danger.M_Danger'")));
}

void AMZ_Stage::BeginPlay()
{
	Super::BeginPlay();

	/* Early return para los materiales. */
	if (!StageBase || !StageBorders)
	{
		return;
	}

	/* Si el nombre no es "L_One_Dark" */
	if (StageMaterial)
	{
		/* Seteo el material del stage. */
		SettingStageMaterials(StageMaterial, FName(TEXT("Beginning")));
	}
	else 
	{
		/* Early return para el material por defecto. */
		if (!DefaultMaterial)
		{
			return;
		}

		/* Seteo el material por defecto al stage. */
		SettingStageMaterials(DefaultMaterial, FName(TEXT("Beginning")));
	}	
}

void AMZ_Stage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
			SettingStageMaterials(CollisionMaterial, FName(TEXT("Overlapping")));
			PlayActivatingSound();
			HandleScalingProgress(FName(TEXT("Decreasing")));
			bIsActivated = true;
		}
	}
}

void AMZ_Stage::HandleScalingProgress(FName Situation)
{
	/* Obtengo la escala de partida del componente base del stage. */
	InitialScale = StageBase->GetRelativeScale3D();

	/* Inicializo el tiempo transcurrido del timer. */
	ElapsedTime = 0.0f;

	if (Situation.IsEqual(FName(TEXT("Decreasing"))))
	{
		/* Seteo el tiempo de animación. */
		AnimationTime = 1.0f;

		/* Seteo la escala final a 0. */
		FinalScale = FVector(0.0f, 0.0f, 0.0f);

		/* Seteo el delay para el decrecimiento. */
		DelayAmount = 1.0f;

		/* Creación del timer. */
		GetWorldTimerManager().SetTimer(
			ScaleTimerHandle,
			this,
			&AMZ_Stage::DecreasingScale,
			0.01f,
			true,
			DelayAmount);
	}
	else 
	{
		if (bIsActivated)
		{
			/* Seteo el tiempo de animación. */
			AnimationTime = 0.3f;

			/* Seteo la escala final a 0.8. */
			FinalScale = FVector(0.8f, 0.8f, 0.8f);

			/* Seteo el delay para el crecimiento. */
			DelayAmount = 0.0f;

			/* Seteo los materiales de la plataforma. */
			StageBase->SetMaterial(0, StageMaterial);
			StageBorders->SetMaterial(0, StageMaterial);

			/* Creación del Timer. */
			GetWorldTimerManager().SetTimer(
				ScaleTimerHandle,
				this,
				&AMZ_Stage::IncreasingScale,
				0.01f,
				true,
				DelayAmount);
		}
	}
}

void AMZ_Stage::DecreasingScale()
{
	/* Sumamos 0.01 segundos al tiempo transcurrido. */
	ElapsedTime += GetWorld()->GetDeltaSeconds();

	/* Clampeamos el valor del tiempo de animación entre el tiempo transcurrido. */
	float Alpha = FMath::Clamp(ElapsedTime / AnimationTime, 0.0f, 1.0f);

	/* Guardamos la nueva escala con uan interpolación lineal entre la inicial y la final. */
	FVector NewScale = FMath::Lerp(InitialScale, FinalScale, Alpha);
	/* Seteamos la escala nueva. */
	StageBase->SetRelativeScale3D(NewScale);

	/* Si el tiempo transcurrido es de más de 1 segundo limpio el timer y notifico que la plataforma está activada. */
	if (Alpha >= 0.90f)
	{
		StageBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StageBorders->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
	}
}

void AMZ_Stage::IncreasingScale()
{
	/* Sumamos 0.01 segundos al tiempo transcurrido. */
	ElapsedTime += GetWorld()->GetDeltaSeconds();

	/* Clampeamos el valor del tiempo de animación entre el tiempo transcurrido. */
	float Alpha = FMath::Clamp(ElapsedTime / AnimationTime, 0.0f, 1.0f);

	/* Guardamos la nueva escala con uan interpolación lineal entre la inicial y la final. */
	FVector NewScale = FMath::Lerp(InitialScale, FinalScale, Alpha);
	/* Seteamos la escala nueva. */
	StageBase->SetRelativeScale3D(NewScale);

	/* Si el tiempo transcurrido es de más de 1 segundo limpio el timer y notifico que la plataforma está activada. */
	if (Alpha >= 1.0f)
	{
		StageBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StageBorders->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StageCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		GetWorldTimerManager().ClearTimer(ScaleTimerHandle);

		bIsActivated = false;
	}
}

void AMZ_Stage::SettingStageMaterials(UMaterialInterface* Material, FName Situation)
{
	if(Situation.IsEqual(FName(TEXT("Beginning"))))
	{
		StageBase->SetMaterial(0, Material);
		StageBorders->SetMaterial(0, Material);
	}
	else 
	{
		StageBase->SetMaterial(0, Material);
	}
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