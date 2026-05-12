// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "PowerUps/StagesRespawner.h"
#include "Mechanics/MZ_Stage.h"
#include "Kismet/GameplayStatics.h"

AStagesRespawner::AStagesRespawner()
{
	// Setea para este actor el evento Tick para cada frame.
	PrimaryActorTick.bCanEverTick = false;

	/* Creo la base del power up y lo seteo como Root. */
	RespawnerBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Respawner Base"));
	SetRootComponent(RespawnerBase);

	/* Creo el mesh de dentro del power up para que indique que es para restablecer las plataformas y lo atacho a la base. */
	RespawnerInsideMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Respawner Inside Mesh"));
	RespawnerInsideMesh->SetupAttachment(RespawnerBase);

	/* Creo la esfera de colisión, la atacho a la base, y genero evento de Overlap Begin para la esfera. */
	RespawnerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Respawner Collision"));
	RespawnerCollision->SetupAttachment(RespawnerBase);
	RespawnerCollision->OnComponentBeginOverlap.AddDynamic(this, &AStagesRespawner::OnOverlapBegin);
}

void AStagesRespawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStagesRespawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStagesRespawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* Si el otro actor es válido y no es ese actor... */
	if (OtherActor && (OtherActor != this))
	{
		/* Si el otro actor tiene la tag "Pawn"... */
		if (OtherActor->ActorHasTag(FName(TEXT("Pawn"))))
		{
			/* Creo un vector para guardar los actores. */
			TArray<AActor*> FoundStages;

			/* Creo una subclase del stage. */
			TSubclassOf<AActor> Stages = AMZ_Stage::StaticClass();

			/* Consigo todos los actores de la clase Stage con el Tag del Respawner de manera que coincidan. */
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), Stages, RespawnerTag, FoundStages);

			/*
			 * Con un for each recorro los actores obtenidos, les hago el casteo a objetos de clase Stage y ejecuto el progreso de escala de
			 * cada uno. 
			*/
			for (AActor* Actor : FoundStages)
			{
				AMZ_Stage* Stage = Cast<AMZ_Stage>(Actor);
				if (Stage)
				{
					Stage->HandleScalingProgress("Increasing");
				}
			}

			/* Después destruimos el power up. */
			this->Destroy();
		}
	}
}

