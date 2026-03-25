// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).


#include "Mechanics/MZ_PlatformManager.h"

#include "Components/ChildActorComponent.h"
#include "Kismet/KismetMathLibrary.h"


AMZ_PlatformManager::AMZ_PlatformManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
}

void AMZ_PlatformManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	/* 1. Limpio los componentes antiguos antes de generar nuevos en caso de que haya. */
	for (UChildActorComponent* Component : PlatformComponents)
	{
		/* Si es válido lo destruyo. */
		if (Component)
		{
			Component->DestroyComponent();
		}
	}

	PlatformComponents.Empty();

	if (PlatformClass == nullptr)
	{
		return;
	}

	/* 2. Obtengo el ancho del actor. (Uso sus Defaults). */
	AActor* DefaultActor = PlatformClass->GetDefaultObject<AActor>();
	FVector Origin, Extent;
	DefaultActor->GetActorBounds(false, Origin, Extent);

	/* Uso el ancho en el eje Y. */
	float ActorWidth = Extent.Y * 2.0f;

	/* 3. Cálculo del radio dinámico. */
	float TotalCircunference = (ActorWidth * NumberOfPlatforms) + Spacing;
	float Radius = TotalCircunference / (2.0f * PI);

	/* 4. Bucle para posicionar las plataformas. */
	for (int32 i = 0; i < NumberOfPlatforms; i++)
	{
		/* Calculo el ángulo en grados y radianes. */
		float AngleDegrees = (360.0f / NumberOfPlatforms) * i;
		float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

		/* Trigonometría básica para la posición local. */
		FVector RelativeLocation(
			FMath::Cos(AngleRadians) * Radius,
			FMath::Sin(AngleRadians) * Radius,
			0.0f
		);

		/* Orientación: Que miren al centro (0,0,0 local). */
		FRotator RelativeRotation = UKismetMathLibrary::FindLookAtRotation(RelativeLocation, FVector::ZeroVector);

		/* Crear y registrar el componente. */
		UChildActorComponent* NewComponent = NewObject<UChildActorComponent>(this);

		NewComponent->RegisterComponent();
		NewComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NewComponent->SetChildActorClass(PlatformClass);
		NewComponent->SetRelativeLocationAndRotation(RelativeLocation, RelativeRotation);

		PlatformComponents.Add(NewComponent);
	}
}

void AMZ_PlatformManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMZ_PlatformManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

