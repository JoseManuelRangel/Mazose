// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/TimeLineComponent.h"

#include "MZ_Spikes.generated.h"

UCLASS()
class MAZOSE_API AMZ_Spikes : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor. */
	AMZ_Spikes();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:	
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

public:
	/* VARIABLES DEL ACTOR */
	/* Static Mesh Base. (Root) */
	UPROPERTY(VisibleAnywhere, Category = "Spikes Structure")
	UStaticMeshComponent* BaseComponent;

	/* Static Mesh de los pinchos (spikes) de la plataforma. */
	UPROPERTY(VisibleAnywhere, Category = "Spikes Structure")
	UStaticMeshComponent* SpikesComponent;

	/* Curva que define el movimiento (Asignable en el editor). */
	UPROPERTY(EditAnywhere, Category = "Spikes Movement")
	UCurveFloat* MovementCurve;

	/* Componente que gestionará el tiempo. */
	UPROPERTY()
	UTimelineComponent* MovementTimelineComponent;

	/* Posición inicial de los pinchos. */
	FVector StartSpikesLocation;

	/* Posición final de los pinchos. */
	FVector EndSpikesLocation;


	/* FUNCIONES DEL ACTOR */
	/* Función que se ejecuta cada frame del Timeline. */
	UFUNCTION()
	void HandleSpikesProgress(float Value);

	/* Función que se ejecuta al finalizar el Timeline. */
	UFUNCTION()
	void OnTimeLineFinished();

	/* Función para el inicio del overlap de los pinchos con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Función que reproduce la Timeline. */
	void SettingTimeline();
};
