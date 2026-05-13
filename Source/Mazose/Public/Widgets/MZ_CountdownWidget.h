// Fill out your copyright notice in the Description page of Project Settings.

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
	/* Funciones de la clase. */
	/* Función que llama al evento del Game Mode que elimina la plataforma de respawn. */
	UFUNCTION(BlueprintCallable, Category = "Logic")
	void OnCountdownAnimationFinished();
};
