// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MZ_CountdownWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAZOSE_API UMZ_CountdownWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/* Evento de construcción inicial del widget. */
	virtual void NativeConstruct() override;

protected:
	/* Variables de la clase. */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SecondsAnimation;

	/* Estructura de Unreal para escuchar finales de animación. */
	FWidgetAnimationDynamicEvent AnimationFinishedDelegate;

protected:
	/* Funciones de la clase. */
	/* Función que llama al evento del Game Mode que elimina la plataforma de respawn. */
	UFUNCTION(BlueprintCallable, Category = "Logic")
	void OnCountdownAnimationFinished();

	/* Función que susituye al Finished del nodo "Play Animation With Finished Event". */
	UFUNCTION()
	void HandleAnimationFinished();
};
