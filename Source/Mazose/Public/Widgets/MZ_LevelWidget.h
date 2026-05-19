// José Manuel Rangel Muñoz. Copyright © Todos los derechos reservados (Excepto algunos assets).

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "Player/BallPlayerController.h"

#include "MZ_LevelWidget.generated.h"


UCLASS()
class MAZOSE_API UMZ_LevelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/* Evento equivalente a Event Construct. */
	virtual void NativeConstruct() override;

	/* Evento tick del widget. */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	/* Variables de la clase. */
	/* Referencia al Player Controller. */
	UPROPERTY()
	ABallPlayerController* Controller;

	/* Textblock de las fresas. */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Strawberries;

	/* Textblock de las monedas. */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Coins;

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

	/* Animación que hace un loop continuo de escala y rotación para las fresas. */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Strawberries;

	/* Animación que hace un loop continuo de escala y rotación para las monedas. */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Coins;

	/* Variable que permite elegir el Blueprint "WB_Score_HUD" en el editor. */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	/* Widget de la puntuación. */
	UPROPERTY()
	UUserWidget* CurrentScoreWidget;

	/* Tiempo que ha transcurrido en la partida. */
	float Time = 0.0f;

	/* Contador de fresas de la partida. */
	int32 StrawberriesCount = 0;

	/* Contador de monedas de la partida. */
	int32 CoinsCount = 0;

	/* Booleano que controla si el jugador está un piso por debajo o no. */
	bool bPlayerIsLower = false;

	/* Booleano que controla si el jugador está vivo o no. */
	bool bPlayerIsAlive = true;

	/* Texto que registra el record del jugador. */
	FText PlayerRecord;

	/* Texto de inicialización del formato del tiempo. */
	FText InitialTime;

	/* Timer para un pequeño delay. */
	FTimerHandle ScoreDelayTimerHandle;

public:
	/* Variables de la clase. */
	/* Funciones de la clase. */
	/* Función para poder actualizar las fresas en el textbox. */
	UFUNCTION()
	void AddingStrawberrries();

	UFUNCTION()
	void AddingCoins();

	/* Función para poder actualizar la posición de la bola en el indicador. */
	UFUNCTION()
	void UpdatingBallIndicatorPosition();

	/* Función para poder actualizar el estado del pawn en partida. */
	UFUNCTION()
	void SettingAliveToFalse();

	/* Función para el Timer. */
	void ActiveTimer();

	/* Función para mostrar el widget de la puntuación. */
	void ShowScoreHUD();
};
