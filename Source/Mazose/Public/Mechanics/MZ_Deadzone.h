// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"

#include "MZ_Deadzone.generated.h"

UCLASS()
class MAZOSE_API AMZ_Deadzone : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor. */
	AMZ_Deadzone();

public:
	/* Variables de la clase. */
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

public:	
	/* Funciones de la clase. */
	/* Función para el inicio del overlap con el Pawn. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
