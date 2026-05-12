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
	FVector FinalScale;

	/* Timer para la reducción o incremento de escala del stage. */
	FTimerHandle ScaleTimerHandle;

	/* Tiempo que durará la animación de desescala. */
	float AnimationTime;

	/* Tiempo que ha transcurrido del timer. */
	float ElapsedTime;

	/* Delay para el timer. */
	float DelayAmount;

	/* Mesh que compone la base del stage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Structure")
	UStaticMeshComponent* StageBase;

	/* Mesh que compone los bordes del stage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Structure")
	UStaticMeshComponent* StageBorders;

	/* Material para la base del stage. */
	UPROPERTY(EditAnywhere, Category = "Stage Materials")
	UMaterialInterface* StageMaterial;

	/* Material que saldrá una vez pisado el stage. */
	UPROPERTY(VisibleAnywhere, Category = "Stage Materials")
	UMaterialInterface* CollisionMaterial;

	/* Material por defecto en caso de no tener asignado ningún material. */
	UPROPERTY(VisibleAnywhere, Category = "Stage Materials")
	UMaterialInterface* DefaultMaterial;

	/* Caja de colisión que va a detectar a la bola entrar en su perímetro. */
	UPROPERTY(EditAnywhere, Category = "Stage Structure")
	UBoxComponent* StageCollision;

	/* Componente de escena para hacer aparecer las fresas. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* StrawberryPosition;
	
	/* Sonido para cuando el jugador pise el stage. */
	UPROPERTY(EditAnywhere, Category = "Stage Audio")
	USoundBase* StageSound;


public:	
	/* Tick: Se llama a cada frame. */
	virtual void Tick(float DeltaTime) override;

	/* Función que maneja tanto la escalada como la desescalada del stage. */
	void HandleScalingProgress(FName Situation);

	/* Función que se encarga de actualizar en tiempo real la desescalada del stage. */
	void DecreasingScale();

	/* Función que se encarga de actualizar en tiempo real el incremento del stage. */
	void IncreasingScale();

	/* Función para setear los materiales en el stage. */
	void SettingStageMaterials(UMaterialInterface* Material, FName Situation);

	/* Función para activar el sonido al pisar el stage. */
	void PlayActivatingSound();

	/* Función para el inicio del overlap del stage con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
