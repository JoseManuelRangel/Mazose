// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MZ_CountdownWidget.h"

#include "MZ_GameMode.h"

#include "Kismet/GameplayStatics.h"


void UMZ_CountdownWidget::OnCountdownAnimationFinished()
{
	/* Obtengo el Game Mode. */
	AGameModeBase* GM = UGameplayStatics::GetGameMode(GetWorld());

	/* Early return para comprobar que el Game Mode existe y es válido. */
	if (!GM)
	{
		return;
	}

	/* Le hago un cast al Game Mode creado por mí. */
	AMZ_GameMode* GameMode = Cast<AMZ_GameMode>(GM);

	/* Early return par comprobar que el Game Mode creado existe y es válido. */
	if (!GameMode)
	{
		return;
	}

	/* Llamo a la función del gamemode para destruir la plataforma de respawn. */
	GameMode->DestroyingRespawnPlatform();

	/* Elimino este widget. */
	this->RemoveFromParent();
}
