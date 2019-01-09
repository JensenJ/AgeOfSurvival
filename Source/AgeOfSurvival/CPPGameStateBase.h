// Copyright Age of Survival 2019.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CPPPlayerController.h"
#include "CPPGameStateBase.generated.h"


//Enum for different seasons.
UENUM(BlueprintType)
enum class ESeasonEnum : uint8 {
	ENone		UMETA(DisplayName = "None"),
	ESpring		UMETA(DisplayName = "Spring"),
	ESummer		UMETA(DisplayName = "Summer"),
	EAutumn		UMETA(DisplayName = "Autumn"),
	EWinter		UMETA(DisplayName = "Winter")
};

UENUM(BlueprintType)
enum class ESuffixEnum : uint8 {
	ENone		UMETA(DisplayName = "None"),
	ECelsius	UMETA(DisplayName = "Celsius"),
	EFahrenheit	UMETA(DisplayName = "Fahrenheit"),
	//EDegrees	UMETA(DisplayName = "Degrees"),
	EDirection	UMETA(DisplayName = "Direction")
};

UCLASS()
class AGEOFSURVIVAL_API ACPPGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	//Constructor
	ACPPGameStateBase();
	//Arrays for time data and date data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TArray<int32> GameTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	TArray<int32> GameDate;
	//Game speed multiplier 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float GameSpeedMultiplier = 1.0f;

	//Environment

	//Boolean for day/night
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	bool bIsNight;

	//Update function for blueprint (visual stuff, e.g. sun position)
	UFUNCTION(BlueprintImplementableEvent, Category = "Calendar")
	void UpdateEnvironment
	(
		FRotator SunAngle, 
		ESeasonEnum Season, 
		const FString& Temperature,
		const FString& Wind, 
		const FString& WindAngle
	);

	//Currently selected season
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	ESeasonEnum SeasonEnum;

	//Boolean for temperature unit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	bool bIsTempFahrenheit;

	//Variables for calculations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float TempFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WindFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WindAngleFloat;

	//Variables for display to the user.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FString TempString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FString WindString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FString WindAngleString;

	//Multipliers for base generated temp/wind.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float TempMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WindMultiplier = 1.0f;

protected:
	//Base functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	//Converts float to string with 1 dp.
	FString FloatToDisplay(float Value, ESuffixEnum Suffix, bool bIncludeDecimal);

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

	//Environment Functions
	void EnvironmentTick();
	ESeasonEnum Season(int32 Month);
	FRotator DayNight();
	float Temperature();
	float Wind();
	float WindAngle();

	//Environment Variables
	float DayNightHours = 0;

	//Temperature
	TArray<float> GameTemp;
	float GeneratedTemp = 5.0f;
	float LastTemp;
	float MinGenTemp;
	float MaxGenTemp;
	float AverageTemp;
	bool bHasGeneratedTemp = false;

	//Wind
	TArray<float> GameWind;
	float GeneratedWind;
	float LastWind;
	float AverageWind;
	bool bHasGeneratedWind = false;

	//Wind Angle
	TArray<float> GameWindAngle;
	float GeneratedWindAngle;
	float LastWindAngle;
	float AverageWindAngle;
	bool bHasGeneratedWindAngle = false;
};
