// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "Blueprint/UserWidget.h"

#include "MZ_GameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoundEvent);

UCLASS()
class MAZOSE_API AMZ_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/* Constructor. */
	AMZ_GameMode();

protected:
	/* Begin Play: Se llama cuando empieza el juego. */
	virtual void BeginPlay() override;

public:
	/* Variables de la clase. */
	/* Variable que permite elegir el Blueprint "WB_Count_HUD" en el editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game UI")
	TSubclassOf<UUserWidget> CountdownWidgetClass;

	/* Variable que mpermite elegir el Blueprint "BP_Respawn_Platform" en el editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setup")
	TSubclassOf<AActor> RespawnPlatformClass;

	/* Widget de la cuenta atrás para empezar a jugar. */
	UPROPERTY()
	UUserWidget* CurrentCountdownWidget;

	/* Audio para poder reproducir en los niveles. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Audio")
	class UAudioComponent* LevelMusic;

	/* Event Dispatcher para darle al play a la música. */
	UPROPERTY(BlueprintAssignable, Category = "Audio Events")
	FOnSoundEvent OnPlaySound;

	/* Event Dispatcher para darle stop a la música. */
	UPROPERTY(BlueprintAssignable, Category = "Audio Events")
	FOnSoundEvent OnStopSound;

public:
	/* Funciones de la clase. */
	/* Función para poder poner la música en el nivel. */
	UFUNCTION()
	void PlayingMusic();

	/* Función para poder quitar la música en el nivel. */
	UFUNCTION()
	void QuittingMusic();

	/* Evento para setear la cuenta atrás. */
	UFUNCTION(BlueprintCallable, Category = "Game Events")
	void CountdownSet();

	/* Función para setear la cuenta atrás en los niveles jugables. */
	void SettingCountdownOnPlayableLevels();

	/* Función para destruir la plataforma de respawn. */
	UFUNCTION(BlueprintCallable, Category = "Game Events")
	void DestroyingRespawnPlatform();
};
