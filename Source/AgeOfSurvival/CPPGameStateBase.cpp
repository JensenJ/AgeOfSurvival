// Copyright Age of Survival 2019.

#include "CPPGameStateBase.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/Actor.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "UObject/ConstructorHelpers.h"

ACPPGameStateBase::ACPPGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void ACPPGameStateBase::BeginPlay() {
	//Limits size of array to 3 to stop overflow
	GameTime.SetNum(3);
	GameDate.SetNum(3);

	//Convert int to float for three main time variables
	Hours = UKismetMathLibrary::Conv_IntToFloat(Hours);
	Minutes = UKismetMathLibrary::Conv_IntToFloat(Minutes) / 60;
	Seconds = UKismetMathLibrary::Conv_IntToFloat(Seconds) / 3600;
	//Set starting time
	float StartingTime = Hours + Minutes + Seconds;
	Clockwork = StartingTime;

	//Sets Game Date variables
	GameDate.Insert(Day, 0);
	GameDate.Insert(Month, 1);
	GameDate.Insert(Year, 2);
}

//Ticks through and updates functions
void ACPPGameStateBase::Tick(float DeltaSeconds) {
	SetClockwork(DeltaSeconds);
	Clock();
	Calendar();
	EnvironmentTick(); //Updates all environment based stuff (day/night, cloud speed, opacity, etc)
}

void ACPPGameStateBase::EnvironmentTick() {
	SeasonEnum = Season(Month);
	FRotator SunAngle = DayNight();
	UpdateEnvironment(SunAngle, SeasonEnum); //Blueprint Function
}

//Sets clockwork for working out game speed.
void ACPPGameStateBase::SetClockwork(float DeltaSeconds) {
	//Works out game speed
	float DeltaTimeUnit = (DeltaSeconds / TimeUnit * 0.24) * GameSpeedMultiplier; 
	float AddedClockwork = DeltaTimeUnit + Clockwork;
	float NewDayTick = AddedClockwork / (60 * 24);
	float Remainder = FGenericPlatformMath::Fmod(AddedClockwork, (60.0f * 24.0f));
	Clockwork = Remainder;
	DayTick = NewDayTick;
}

//Calculates time
void ACPPGameStateBase::Clock() {
	// Seconds
	float ClockworkSeconds = Clockwork * 3600;
	float NewSeconds = ClockworkSeconds / 60;
	float RemainderSeconds = FGenericPlatformMath::Fmod(NewSeconds, 60.0f);
	UKismetMathLibrary::FFloor(RemainderSeconds);
	Seconds = RemainderSeconds;

	// Minutes
	float NewMinutes = NewSeconds / 60;
	float RemainderMinutes = FGenericPlatformMath::Fmod(NewMinutes, 60.0f);
	UKismetMathLibrary::FFloor(RemainderMinutes);
	Minutes = RemainderMinutes;

	// Hours
	float NewHours = NewMinutes / 60;
	DayNightHours = FGenericPlatformMath::Fmod(NewHours, 60.0f);
	UKismetMathLibrary::FFloor(DayNightHours);
	Hours = DayNightHours;

	//Sets game time variables into array
	GameTime.Insert(Seconds, 0);
	GameTime.Insert(Minutes, 1);
	GameTime.Insert(Hours, 2);

	//Logs time and whether day or night
	FString strHours = FString::FromInt(GameTime[2]);
	FString HoursMinutesString = UKismetStringLibrary::BuildString_Int(strHours, ":", GameTime[1], "");
	FString FinalString = UKismetStringLibrary::BuildString_Int(HoursMinutesString, ":", GameTime[0], "");
	UE_LOG(LogTemp, Warning, TEXT("PEGameStateBase: Time: %s"), *FinalString);
	UE_LOG(LogTemp, Warning, TEXT("Night: %s"), (bIsNight ? TEXT("True") : TEXT("False")));
}

//Returns the season based on what month it is
ESeasonEnum ACPPGameStateBase::Season(int32 Month) {
	if (Month == 12 || Month == 1 || Month == 2) {
		return ESeasonEnum::EWinter;
	}
	else if (Month == 3 || Month == 4 || Month == 5) {
		return ESeasonEnum::ESpring;
	}
	else if (Month == 6 || Month == 7 || Month == 8) {
		return ESeasonEnum::ESummer;
	}
	else if (Month == 9 || Month == 10 || Month == 11) {
		return ESeasonEnum::EAutumn;
	}
	else {
		return ESeasonEnum::ENone;
	}
}

//Calculates SunAngle and returns to environment tick
FRotator ACPPGameStateBase::DayNight() {
	float SunAngle = ((DayNightHours / 6) * 90) + 90;
	FRotator SunRot = UKismetMathLibrary::MakeRotator(0, SunAngle, 90);

	//Day-Night Toggle
	if (Hours >= 17 || Hours <= 5) {
		bIsNight = true;
	}
	else {
		bIsNight = false;
	}

	return SunRot;
}

//Calculates date
void ACPPGameStateBase::Calendar() {
	Day = DayTick + Day;
	int32 DaysInMonth = UKismetMathLibrary::DaysInMonth(Year, Month);
	if (Day > DaysInMonth) {
		Day = 1;
		Month++;
	}
	if (Month > 12) {
		Month = 1;
		Year++;
	}

	//Sets Game Date variables into array
	GameDate.Insert(Day, 0);
	GameDate.Insert(Month, 1);
	GameDate.Insert(Year, 2);

	//Logs date
	FString strDays = FString::FromInt(GameDate[0]);
	FString DaysMonthString = UKismetStringLibrary::BuildString_Int(strDays, "/", GameDate[1], "");
	FString FinalString = UKismetStringLibrary::BuildString_Int(DaysMonthString, "/", GameDate[2], "");
	UE_LOG(LogTemp, Warning, TEXT("PEGameStateBase: Date: %s"), *FinalString);
}