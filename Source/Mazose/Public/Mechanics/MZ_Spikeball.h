// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"

#include "MZ_Spikeball.generated.h"

UCLASS()
class MAZOSE_API AMZ_Spikeball : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor. */
	AMZ_Spikeball();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:
	/* Variables de la clase */
	/* Mesh que le da forma al actor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spikeball Structure")
	UStaticMeshComponent* BallBase;

	/* Esfera de colisión para el actor. */
	UPROPERTY(EditAnywhere, Category = "Spikeball Structure")
	USphereComponent* BallCollision;

	/* Timeline para la patrulla que seguirá el actor. */
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* PatrolTimeline;

	/* Curva que servirá para el timeline de patrulla. */
	UPROPERTY(EditAnywhere, Category = "Spikeball Patrol Settings")
	UCurveFloat* PatrolCurve;

	UPROPERTY(EditAnywhere, Category = "Spikeball Patrol Settings")
	TArray<FVector> PatrolWaypoints;

	/* Fuerza de repulsión al chocar con el Pawn. */
	UPROPERTY(EditAnywhere, Category = "Spikeball Repulsion")
	float ImpulseStrength = 2000.0f;

	FTransform StartTransform;

	/* Vector que determina la localización inicial de la bola. */
	FVector StartLocation;

	/* Vector que determina la localización a alcanzar de la bola. */
	FVector TargetLocation;

	/* Índice que muestra el trazo de camino que está realizando la bola. */
	int32 CurrentIndex = 0;

public:	
	/* Eventos de la clase. */
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

	/* Overlap: Se llama al sobrepasar este actor con otro. */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/* Funciones de la clase. */
	/* Maneja el tiempo de la timeline y progresa en la funcionalidad que se le dé según el valor de la variable de entrada. */
	UFUNCTION()
	void HandleTimelineProgress(float Value);

	/* Función que se ejecuta cuando la bola alcanza uno de los tramos de la patrulla. */
	UFUNCTION()
	void OnWaypointReached();
};
