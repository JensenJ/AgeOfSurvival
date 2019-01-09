// Copyright Age of Survival 2019.

#include "CPPGameStateBase.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/Actor.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetTextLibrary.h"

ACPPGameStateBase::ACPPGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;
}

void ACPPGameStateBase::BeginPlay() {
	//Limits size of array to 3 to stop overflow
	GameTime.SetNum(3);
	GameDate.SetNum(3);
	GameTemp.SetNum(3);
	GameWind.SetNum(3);
	GameWindAngle.SetNum(3);

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
//Ticks through and updates functions related to environment
void ACPPGameStateBase::EnvironmentTick() {
	SeasonEnum = Season(Month);
	FRotator SunAngle = DayNight();
	WindFloat = Wind();
	WindAngleFloat = WindAngle();
	TempFloat = Temperature();
	if (bIsTempFahrenheit) {
		TempString = FloatToDisplay(TempFloat, ESuffixEnum::EFahrenheit, true);
	}
	else {
		TempString = FloatToDisplay(TempFloat, ESuffixEnum::ECelsius, true);
	}

	WindString = FloatToDisplay(WindFloat, ESuffixEnum::ENone, false);
	WindAngleString = FloatToDisplay(WindAngleFloat, ESuffixEnum::EDirection, false);

	sunBrightness = SunBrightness();
	cloudSpeed = WindFloat;
	cloudOpacity = CloudOpacity();
	starOpacity = StarOpacity();

	UpdateEnvironment(SunAngle, SeasonEnum, *TempString, *WindString, *WindAngleString, sunBrightness, cloudSpeed, cloudOpacity, starOpacity); //Blueprint Function
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
		UE_LOG(LogTemp, Error, TEXT("Season::Setting season failed!"));
		return ESeasonEnum::ENone;
	}
}

//Calculates SunAngle and returns to environment tick
FRotator ACPPGameStateBase::DayNight() {
	float SunAngle = ((DayNightHours / 6) * 90) + 90;
	FRotator SunRot = UKismetMathLibrary::MakeRotator(0, SunAngle, 90);

	//Day-Night Toggle
	if (Hours >= 18 || Hours <= 5) {
		bIsNight = true;
	}
	else {
		bIsNight = false;
	}

	return SunRot;
}
//Generates temperature for this hour.
float ACPPGameStateBase::Temperature() {
	if (bNewGenerationTemp) {
		LastTemp = FMath::RandRange(0.0f, 7.0f);
		bNewGenerationTemp = false;
	}

	if (GameTime[1] == 0) { //Resets temperature every hour when minute is 0 (new hour)
		if (!bHasGeneratedTemp) {
			for (int i = 0; i < 3; i++) { //Iterations for getting an average

				//Sets low and high bounds for each season
				if (SeasonEnum == ESeasonEnum::EWinter) {
					MaxGenTemp = 8.0f * TempMultiplier;
					MinGenTemp = -7.0f * TempMultiplier;
				}
				else if (SeasonEnum == ESeasonEnum::ESpring) {
					MaxGenTemp = 12.0f * TempMultiplier;
					MinGenTemp = 2.0f * TempMultiplier;
				}
				else if (SeasonEnum == ESeasonEnum::ESummer) {
					MaxGenTemp = 20.0f * TempMultiplier;
					MinGenTemp = 7.0f * TempMultiplier;
				}
				else if (SeasonEnum == ESeasonEnum::EAutumn) {
					MaxGenTemp = 13.0f * TempMultiplier;
					MinGenTemp = -3.0f * TempMultiplier;
				}
				else {
					//Error logging
					UE_LOG(LogTemp, Error, TEXT("Temperature::Check for season failed!"));
				}

				//Generate base temperature
				GeneratedTemp = FMath::RandRange(MinGenTemp, MaxGenTemp);

				//Makes sure temperature between last and current is not too far apart.
				if ((GeneratedTemp - LastTemp) > 4) {
					GeneratedTemp = LastTemp + FMath::RandRange(2.0f, 3.5f);
				}
				else if ((LastTemp - GeneratedTemp) > 4) {
					GeneratedTemp = LastTemp - FMath::RandRange(0.0f, 2.5f);
				}

				//Gradual increase towards midday
				if (GameTime[2] <= 13 && GameTime[2] > 1) {
					GeneratedTemp = GeneratedTemp + FMath::RandRange(1.5f, 3.0f);
				}
				//Gradual decrease towards midnight
				else if (GameTime[2] > 13 && GameTime[2] < 24) {
					GeneratedTemp = GeneratedTemp - FMath::RandRange(0.2f, 1.5f);
				}
				GameTemp.Insert(GeneratedTemp, i);
			}

			//Calculating Mean Temperature
			AverageTemp = (GameTemp[0] + GameTemp[1] + GameTemp[2]) / 3;

			AverageTemp = AverageTemp - WindFloat;

			if (AverageTemp < -273.0f) {
				AverageTemp = -273.0f;
			}
			LastTemp = AverageTemp; //Setting last temp for next hour.
			//Converting celsius to fahrenheit if user has option enabled.
			if (bIsTempFahrenheit) {
				AverageTemp = (AverageTemp * (9 / 5)) + 32;
			}

			bHasGeneratedTemp = true; //Makes sure generation only happens once
		}
	}
	else {
		bHasGeneratedTemp = false; //Resets the variable for the next hour
	}

	return AverageTemp; //Returns generated temp
}
//Function to return string version of temperature for display.

float ACPPGameStateBase::Wind() {
	if (bNewGenerationWind) {
		LastWind = FMath::RandRange(0.0f, 7.0f);
		bNewGenerationWind = false;
	}

	if (GameTime[1] == 0) { //Resets temperature every hour when minute is 0 (new hour)
		if (!bHasGeneratedWind) {
			for (int i = 0; i < 3; i++) { //Iterations for getting an average
				//Generate base wind
				GeneratedWind = FMath::RandRange(-3.0f, 3.0f);

				//Makes sure wind speed between last and current is not too far apart.
				if ((GeneratedWind - LastWind) > 3) {
					GeneratedWind = LastWind + FMath::RandRange(0.0f, 3.0f);
				}
				else if ((LastWind - GeneratedWind) < 3) {
					GeneratedWind = LastWind - FMath::RandRange(0.0f, 3.0f);
				}

				GameWind.Insert(GeneratedWind, i);
			}

			//Calculating Mean Wind
			AverageWind = (GameWind[0] + GameWind[1] + GameWind[2]) / 3;

			if (AverageWind > 2.5f) {
				AverageWind = AverageWind - FMath::RandRange(0.3f, 0.9f);
			}
			else if (AverageWind < 0) {
				AverageWind = FMath::RandRange(0.3f, 1.3f);
			}

			LastWind = AverageWind;
			bHasGeneratedWind = true; //Makes sure generation only happens once
		}
	}
	else {
		bHasGeneratedWind = false; //Resets the variable for the next hour
	}

	return AverageWind; //Returns generated wind
}

float ACPPGameStateBase::WindAngle() {
	if (bNewGenerationWindAngle) {
		LastWindAngle = FMath::RandRange(0.0f, 360.0f);
		bNewGenerationWindAngle = false;
	}

	if (GameTime[1] == 0) { //Resets temperature every hour when minute is 0 (new hour)
		if (!bHasGeneratedWindAngle) {
			for (int i = 0; i < 3; i++) { //Iterations for getting an average
				GeneratedWindAngle = FMath::RandRange(0.0f, 360.0f);

				//Makes sure wind angle between last and current is no too far apart.
				if ((GeneratedWindAngle - LastWindAngle) > 15) {
					GeneratedWindAngle = LastWindAngle + FMath::RandRange(0.0f, 10.0f);
				}
				else if ((LastWindAngle - GeneratedWindAngle) < 15) {
					GeneratedWindAngle = LastWindAngle - FMath::RandRange(0.0f, 10.0f);
				}
				GameWindAngle.Insert(GeneratedWindAngle, i);
			}

			//Calculating Mean Wind
			AverageWindAngle = (GameWindAngle[0] + GameWindAngle[1] + GameWindAngle[2]) / 3;

			if (AverageWindAngle < 0) {
				AverageWindAngle = 340 - AverageWindAngle;
			}
			else if (AverageWindAngle > 360) {
				AverageWindAngle = AverageWindAngle - 340;
			}
			LastWindAngle = AverageWindAngle;
			bHasGeneratedWindAngle = true; //Makes sure generation only happens once
		}
	}
	else {
		bHasGeneratedWindAngle = false; //Resets the variable for the next hour
	}

	return AverageWindAngle; //Returns generated wind
}

float ACPPGameStateBase::SunBrightness() {
	float brightness = 10.0f;
	if (SeasonEnum == ESeasonEnum::ESpring) {
		brightness *= 0.8;
	}
	else if (SeasonEnum == ESeasonEnum::ESummer) {
		brightness *= 1.2;
	}
	else if (SeasonEnum == ESeasonEnum::EAutumn) {
		brightness *= 0.7;
	}
	else if (SeasonEnum == ESeasonEnum::EWinter) {
		brightness *= 0.5;
	}
	return brightness;
}

float ACPPGameStateBase::CloudOpacity() {
	return 1.0f;
}

float ACPPGameStateBase::StarOpacity() {
	return 2.0f - cloudOpacity;
}

FString ACPPGameStateBase::FloatToDisplay(float Value, ESuffixEnum Suffix, bool bIncludeDecimal) {
	//Converts float to string
	FString String = FString::SanitizeFloat(Value);

	//Finds decimal point index
	int32 DecimalPos = UKismetStringLibrary::FindSubstring(String, ".", false, false, 0);
	//Gets all numbers after decimal
	FString DecimalString = UKismetStringLibrary::GetSubstring(String, DecimalPos, 2);
	//Gets everything before decimal
	FString NumberString = UKismetStringLibrary::LeftChop(String, String.Len() - DecimalPos);

	//UE_LOG(LogTemp, Warning, TEXT("Temp: %f"), TempFloat);
	//UE_LOG(LogTemp, Warning, TEXT("Temp: %s%s"), *NumberString, *DecimalString);

	//Merges before and after decimal together
	FString FinalString;
	if (bIncludeDecimal) {
		FinalString = NumberString.Append(DecimalString);
	}
	else {
		FinalString = NumberString;
	}

	//Adds suffix
	if (Suffix == ESuffixEnum::ECelsius) {
		FinalString = FinalString.Append("C");
	}
	else if (Suffix == ESuffixEnum::EFahrenheit) {
		FinalString = FinalString.Append("F");
	}
	//else if (Suffix == ESuffixEnum::EDegrees) {
	//	FinalString = FinalString.Append("°");
	//}
	else if (Suffix == ESuffixEnum::EDirection) {

		//Appends direction to end of angle based on value. 
		if (Value < 0 || Value > 360) {
			UE_LOG(LogTemp, Error, TEXT("FloatToDisplay::Value is %f, when only values 0 - 360 are valid."), Value)
		}
		else if (Value < 335 && Value > 291) {
			FinalString = FinalString.Append(" (NW)");
		}
		else if (Value < 290 && Value > 246) {
			FinalString = FinalString.Append(" (W) ");
		}
		else if (Value < 245 && Value > 201) {
			FinalString = FinalString.Append(" (SW)");
		}
		else if (Value < 200 && Value > 156) {
			FinalString = FinalString.Append(" (S) ");
		}
		else if (Value < 155 && Value > 111) {
			FinalString = FinalString.Append(" (SE)");
		}
		else if (Value < 110 && Value > 66) {
			FinalString = FinalString.Append(" (E) ");
		}
		else if (Value < 65 && Value > 26) {
			FinalString = FinalString.Append(" (NE)");
		}
		else {
			FinalString = FinalString.Append(" (N) ");
		}
	}
	return FinalString; //Returns string out of function.
}