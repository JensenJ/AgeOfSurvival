// Copyright Age of Survival 2019.

#include "CPPGameStateBase.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/Actor.h"
#include "GenericPlatform//GenericPlatformMath.h"

ACPPGameStateBase::ACPPGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void ACPPGameStateBase::BeginPlay() {
	GameTime.SetNum(3); // Limits size of array to 3
	GameDate.SetNum(3); // Limits size of array to 3

	Hours = UKismetMathLibrary::Conv_IntToFloat(Hours);
	Minutes = UKismetMathLibrary::Conv_IntToFloat(Minutes) / 60;
	Seconds = UKismetMathLibrary::Conv_IntToFloat(Seconds) / 3600;
	float StartingTime = Hours + Minutes + Seconds;
	Clockwork = StartingTime;

	GameDate.Insert(Day, 0);
	GameDate.Insert(Month, 1);
	GameDate.Insert(Year, 2);
}

void ACPPGameStateBase::Tick(float DeltaSeconds) {
	SetClockwork(DeltaSeconds);
	Clock();
	Calendar();
}

void ACPPGameStateBase::SetClockwork(float DeltaSeconds) {
	float DeltaTimeUnit = DeltaSeconds / TimeUnit * GameSpeedMultiplier;
	float AddedClockwork = DeltaTimeUnit + Clockwork;
	float NewDayTick = AddedClockwork / (60 * 24);
	float Remainder = FGenericPlatformMath::Fmod(AddedClockwork, (60.0f * 24.0f));
	Clockwork = Remainder;
	DayTick = NewDayTick;
}

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
	float RemainderHours = FGenericPlatformMath::Fmod(NewHours, 60.0f);
	UKismetMathLibrary::FFloor(RemainderHours);
	Hours = RemainderHours;
	GameTime.Insert(Seconds, 0);
	GameTime.Insert(Minutes, 1);
	GameTime.Insert(Hours, 2);

	FString strHours = FString::FromInt(GameTime[2]);
	FString HoursMinutesString = UKismetStringLibrary::BuildString_Int(strHours, ":", GameTime[1], "");
	FString FinalString = UKismetStringLibrary::BuildString_Int(HoursMinutesString, ":", GameTime[0], "");
	UE_LOG(LogTemp, Warning, TEXT("PEGameStateBase: Time: %s"), *FinalString);
}
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
	GameDate.Insert(Day, 0);
	GameDate.Insert(Month, 1);
	GameDate.Insert(Year, 2);

	FString strDays = FString::FromInt(GameDate[0]);
	FString DaysMonthString = UKismetStringLibrary::BuildString_Int(strDays, "/", GameDate[1], "");
	FString FinalString = UKismetStringLibrary::BuildString_Int(DaysMonthString, "/", GameDate[2], "");
	UE_LOG(LogTemp, Warning, TEXT("PEGameStateBase: Date: %s"), *FinalString);
}