// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Components/TimeLineComponent.h"

#include "Sound/SoundBase.h"

#include "MZ_Stage.generated.h"

UCLASS()
class MAZOSE_API AMZ_Stage : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor. */
	AMZ_Stage();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:
	/* Variables de la clase */
	/* Booleano que controla si la plataforma ya ha sido activada o no */
	bool bIsActivated = false;

	/* Vector que guarda la escala inicial del stage. */
	FVector InitialScale;

	/* Vector que representa la escala final del stage la cual es 0. */
	FVector FinalScale = FVector(0.0f, 0.0f, 0.0f);

	/* Timer que controla la reducción de escala del stage. */
	FTimerHandle ScaleTimerHandle;

	/* Tiempo que durará la animación de desescala. */
	float AnimationTime = 1.0f;

	/* Tiempo que ha transcurrido del timer. */
	float ElapsedTime = 0.0f;

	/* Mesh que compone la base del stage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Structure")
	UStaticMeshComponent* StageBase;

	/* Material para la base del stage. */
	UPROPERTY(EditAnywhere, Category = "Stage Materials")
	UMaterial* StageMaterial;

	/* Mesh que compone los bordes del stage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Structure")
	UStaticMeshComponent* StageBorders;

	/* Caja de colisión que va a detectar a la bola entrar en su perímetro. */
	UPROPERTY(EditAnywhere, Category = "Stage Structure")
	UBoxComponent* StageCollision;
	
	/* Material que saldrá una vez pisado el stage. */
	UPROPERTY(EditAnywhere, Category = "Stage Materials")
	UMaterial* StageCollisionMaterial;

	/* Sonido para cuando el jugador pise el stage. */
	UPROPERTY(EditAnywhere, Category = "Stage Audio")
	USoundBase* StageSound;


public:	
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

	/* Función para activar el sonido al pisar el stage. */
	void PlayActivatingSound();

	/* Función que maneja la transición de escalas del stage. */
	void HandleScaleDecreaseProgress();

	/* Función que se encarga de actualizar en tiempo real la escala del stage. */
	void UpdateScale();

	/* Función para el inicio del overlap del stage con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
