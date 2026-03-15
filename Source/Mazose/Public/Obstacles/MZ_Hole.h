// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/TimeLineComponent.h"

#include "MZ_Hole.generated.h"

UCLASS()
class MAZOSE_API AMZ_Hole : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor. */
	AMZ_Hole();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

public:
	/* VARIABLES DEL ACTOR */
	/* Static Mesh Base. (Root) */
	UPROPERTY(VisibleAnywhere, Category = "Hole Structure")
	UStaticMeshComponent* BaseComponent;

	/* Static Mesh de la primera trampilla (hatch) del agujero. */
	UPROPERTY(VisibleAnywhere, Category = "Hole Structure")
	UStaticMeshComponent* FirstHatchPartComponent;

	/* Rotación inicial de la primera trampilla. */
	FRotator StartFirstHatchRotation;

	/* Rotación final de la primera trampilla. */
	FRotator EndFirstHatchRotation;

	/* Static Mesh de la primera trampilla (hatch) del agujero. */
	UPROPERTY(VisibleAnywhere, Category = "Hole Structure")
	UStaticMeshComponent* SecondHatchPartComponent;

	/* Posición inicial de la primera trampilla. */
	FRotator StartSecondHatchRotation;

	/* Posición final de la primera trampilla. */
	FRotator EndSecondHatchRotation;

	/* Componente de colisión que se situa en el fondo del agujero. */
	UPROPERTY(VisibleAnywhere, Category = "Hole Structure")
	USphereComponent* CollisionComponent;

	/* Curva que define el movimiento (Asignable en el editor). */
	UPROPERTY(EditAnywhere, Category = "Hole Movement")
	UCurveFloat* MovementCurve;

	/* Componente que gestionará el tiempo. */
	UPROPERTY()
	UTimelineComponent* MovementTimelineComponent;


	/* FUNCIONES DEL ACTOR */
	/* Función que se ejecuta cada frame del Timeline. */
	UFUNCTION()
	void HandleHoleProgress(float Value);

	/* Función que se ejecuta al finalizar el Timeline. */
	UFUNCTION()
	void OnTimeLineFinished();

	/* Función para el inicio del overlap con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Función que activa la Timeline. */
	void SettingTimeline();

	/* Función que desactiva los controles del Pawn. */
	void DeactivatePawnInput();
};
