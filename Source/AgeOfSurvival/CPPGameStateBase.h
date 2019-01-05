// Copyright Age of Survival 2019.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CPPPlayerController.h"
#include "CPPGameStateBase.generated.h"

UCLASS()
class AGEOFSURVIVAL_API ACPPGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACPPGameStateBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TArray<int32> GameTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TArray<int32> GameDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float GameSpeedMultiplier;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	//int32 Currency

	//UPROPERTY(BlueprintCallable, Category = "Resources")
	//int32 ChangeCurrency(int32 amount);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:

	// Clock Functions
	void SetClockwork(float DeltaSeconds);
	void Clock();
	void Calendar();
	// Clock Variables
	float TimeUnit = 0.25f;
	float Clockwork = 0.0f;
	float DayTick = 0;
	int32 Seconds = 0;
	int32 Minutes = 0;
	int32 Hours = 0;
	int32 Day = 1;
	int32 Month = 1;
	int32 Year = 1;


};
