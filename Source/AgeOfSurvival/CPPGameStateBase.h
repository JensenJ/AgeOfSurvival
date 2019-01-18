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
	EDirection	UMETA(DisplayName = "Direction"),
	EPercent	UMETA(DisplayName = "Percent")
};

UENUM(BlueprintType)
enum class EWeatherEnum : uint8 {
	ENone		UMETA(DisplayName = "None"),
	ESunny		UMETA(DisplayName = "Sunny"),
	EOvercast	UMETA(DisplayName = "Overcast"),
	ECloudy		UMETA(DisplayName = "Cloudy"),
	ERain		UMETA(DisplayName = "Rain"),
	ESnow		UMETA(DisplayName = "Snow"),
	EThunder	UMETA(DisplayName = "Thunderstorm"),
	EFog		UMETA(DisplayName = "Fog")
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

	//Environment

	//Boolean for day/night
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	bool bIsNight;

	//Update function for blueprint (visual stuff, e.g. sun position)
	UFUNCTION(BlueprintImplementableEvent, Category = "Calendar")
	void UpdateEnvironment();

	//Currently selected season
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	ESeasonEnum SeasonEnum;

	//Boolean for temperature unit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	bool bIsTempFahrenheit;

	//Variables for calculations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float TempFloat;
	//Wind
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WindFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WindAngleFloat;

	//Celestial
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FRotator SunAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float sunBrightness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float cloudSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float cloudOpacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float starOpacity;

	//Weather
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	EWeatherEnum WeatherEnum;

	//Variables for display to the user.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FString TempString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FString WindString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	FString WindAngleString;

	//Multipliers.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Calendar")
	float GameSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float TempMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WindMultiplier = 1.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skybox")

	//Skybox Colours for different weathers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skybox")
	FLinearColor ZenithColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skybox")
	FLinearColor HorizonColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skybox")
	FLinearColor CloudColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skybox")
	FLinearColor OverallColor;
	
protected:
	//Base functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	//Converts float to string with 1 dp.
	FString FloatToDisplay(float Value, ESuffixEnum Suffix, bool bIncludeDecimal, int32 Precision);

	// Clock Functions
	void SetClockwork(float DeltaSeconds);
	void Clock();
	void Calendar();

	// Clock Variables
	float TimeUnit = 0.25f;
	float Clockwork;
	float DayTick = 0;
	int32 Seconds = 0;
	int32 Minutes = 0;
	int32 Hours = 8 * 60;
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
	float SunBrightness();
	float CloudOpacity();
	float StarOpacity();
	EWeatherEnum Weather();
	void SkyboxColour();
	

	//DayNight
	float DayNightHours = 0;

	//Temperature
	TArray<float> GameTemp;
	float GeneratedTemp = 5.0f;
	float LastTemp;
	float MinGenTemp;
	float MaxGenTemp;
	float AverageTemp;
	bool bHasGeneratedTemp = false;
	bool bNewGenerationTemp = true; 

	//Wind
	TArray<float> GameWind;
	float GeneratedWind;
	float LastWind;
	float AverageWind;
	bool bHasGeneratedWind = false;
	bool bNewGenerationWind = true;

	//Wind Angle
	TArray<float> GameWindAngle;
	float GeneratedWindAngle;
	float LastWindAngle;
	float AverageWindAngle;
	bool bHasGeneratedWindAngle = false;
	bool bNewGenerationWindAngle = true;

	//Weather
	TArray<EWeatherEnum> LastWeather;
	EWeatherEnum weather;
	int32 WeatherInt;
	int32 Counter = 0;
	bool bHasGeneratedWeather = false;
	bool bNewGenerationWeather = true;
	bool bIsSnowEnabled = false;
};
