// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"

#include "MZ_Changezone.generated.h"

UCLASS()
class MAZOSE_API AMZ_Changezone : public AActor
{
	GENERATED_BODY()
	
public:	
	/* Constructor. */
	AMZ_Changezone();

protected:
	/* Variables de la clase. */
	/* Caja de colisión para el actor. */
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionComponent;

protected:
	/* Funciones de la clase. */
	/* Evento de overlap. */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
