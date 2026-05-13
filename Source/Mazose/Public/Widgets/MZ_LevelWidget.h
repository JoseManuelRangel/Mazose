// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "MZ_LevelWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddingStrawberries);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateBallIndicatorPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingIsAliveToFalse);

UCLASS()
class MAZOSE_API UMZ_LevelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/* Evento equivalente a Event Construct. */
	virtual void NativeConstruct() override;

protected:
	/* Variables de la clase. */
	/* Textblock de las fresas. */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Strawberries;

	/* Textblock del tiempo. */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Time;

	/* Textblock del récord actual. */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Record_Time;

	/* Animación de bajar la bola en el indicador de altura. */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BallIndicatorLevel;

	/* Animación de aparición de la flecha que indica que la bola del indicador baja. */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BallArrowIndicator;

	/* Animación que realiza un Fade al terminar la partida. */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Fade;

	/* Event Dispatcher para añadir fresas al textbox. */
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnAddingStrawberries OnAddingStrawberries;

	/* Event Dispatcher para actualizar la posición del indicador de altura. */
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnUpdateBallIndicatorPosition OnUpdateBallIndicatorPosition;

	/* Event Dispatcher para actualizar el estado del personaje. */
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnSettingIsAliveToFalse OnSettingIsAliveToFalse;

	/* Variable que permite elegir el Blueprint "WB_Score_HUD" en el editor. */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	/* Widget de la puntuación. */
	UPROPERTY()
	UUserWidget* CurrentScoreWidget;

	/* Tiempo que ha transcurrido en la partida. */
	float Time;

	/* Contador de fresas de la partida. */
	int32 StrawberriesCount = 0;

	/* Booleano que controla si el jugador está un piso por debajo o no. */
	bool bPlayerIsLower = false;

	/* Booleano que controla si el jugador está vivo o no. */
	bool bPlayerIsAlive = true;

	/* Texto que registra el record del jugador. */
	FText PlayerRecord;

	/* Texto de inicialización del formato del tiempo. */
	FText InitialTime;

public:
	/* Funciones de la clase. */
	/* Función para poder actualizar las fresas en el textbox. */
	UFUNCTION()
	void AddingStrawberrries();

	/* Función para poder actualizar la posición de la bola en el indicador. */
	UFUNCTION()
	void UpdatingBallIndicatorPosition();

	/* Función para poder actualizar el estado del pawn en partida. */
	UFUNCTION()
	void SettingAliveToFalse();
};
