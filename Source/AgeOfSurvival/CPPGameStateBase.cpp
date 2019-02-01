// Copyright Age of Survival 2019.

#include "CPPGameStateBase.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/Actor.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetTextLibrary.h"
#include "Math/Color.h"

ACPPGameStateBase::ACPPGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("DataTable'/Game/Data/ItemDB.ItemDB'"));
	ItemDB = BP_ItemDB.Object;
}

UDataTable* ACPPGameStateBase::GetItemDB() const {
	return ItemDB;
}

void ACPPGameStateBase::BeginPlay() {
	//Limits size of array to 3 to stop overflow
	GameTime.SetNum(3);
	GameDate.SetNum(3);
	GameTemp.SetNum(3);
	GameWind.SetNum(3);
	GameWindAngle.SetNum(3);
	LastWeather.SetNum(24);

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
	SunAngle = DayNight();
	WindFloat = Wind();
	WindAngleFloat = WindAngle();
	TempFloat = Temperature();
	if (bIsTempFahrenheit) {
		TempString = FloatToDisplay(TempFloat, ESuffixEnum::EFahrenheit, true, 1);
	}
	else {
		TempString = FloatToDisplay(TempFloat, ESuffixEnum::ECelsius, true, 1);
	}

	WindString = FloatToDisplay(WindFloat, ESuffixEnum::ENone, false, 0);
	WindAngleString = FloatToDisplay(WindAngleFloat, ESuffixEnum::EDirection, false, 0);

	WeatherEnum = Weather();

	sunBrightness = SunBrightness();
	cloudSpeed = WindFloat;
	cloudOpacity = CloudOpacity();
	starOpacity = StarOpacity();

	SkyboxColour();
	UpdateEnvironment(); //Blueprint Function
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
	UE_LOG(LogTemp, Warning, TEXT("CPPGameStateBase: Time: %s"), *FinalString);
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
	UE_LOG(LogTemp, Warning, TEXT("CPPGameStateBase: Date: %s"), *FinalString);
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

			AverageTemp = AverageTemp - WindFloat / 5;

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

float ACPPGameStateBase::Wind() {
	float value = 1;
	if (bNewGenerationWind) {
		LastWind = FMath::RandRange(0.0f, 7.0f);
		bNewGenerationWind = false;
	}

	if ((GameTime[2] == 0 && GameTime[1] == 0) || 
		(GameTime[2] == 6 && GameTime[1] == 0) || 
		(GameTime[2] == 12 && GameTime[1] == 0) || 
		(GameTime[2] == 18 && GameTime[1] == 0)) { //Resets wind every 6th hour when minute is 0 (new hour)
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

			//AverageWind = AverageWind;

			LastWind = AverageWind;
			bHasGeneratedWind = true; //Makes sure generation only happens once

		}
	}
	else {
		bHasGeneratedWind = false; //Resets the variable for the next hour
	}
	value = FMath::Lerp(WindFloat, AverageWind, GameSpeedMultiplier / (60 * 24));
	return value; //Returns generated wind
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
	float NewValue = 1;
	float value = 1;
	if (WeatherEnum == EWeatherEnum::ECloudy) {
		NewValue = 1.75;
	}
	else if (WeatherEnum == EWeatherEnum::ESunny) {
		NewValue = 0.8;
	}
	else if (WeatherEnum == EWeatherEnum::EOvercast) {
		NewValue = 3.0;
	}
	else if (WeatherEnum == EWeatherEnum::ESnow) {
		NewValue = 2.0;
	}
	else if (WeatherEnum == EWeatherEnum::ERain) {
		NewValue = 3.5;
	}
	else if (WeatherEnum == EWeatherEnum::EThunder) {
		NewValue = 4.0;
	}
	else {
		NewValue = 1.0;
	}

	value = FMath::Lerp(cloudOpacity, NewValue, GameSpeedMultiplier / (60 * 24));
	return value;
}

float ACPPGameStateBase::StarOpacity() {
	float NewValue = 1;
	float value = 1;
	if (WeatherEnum == EWeatherEnum::ECloudy) {
		NewValue = 0.6;
	}
	else if (WeatherEnum == EWeatherEnum::ESunny) {
		NewValue = 1.0;
	}
	else if (WeatherEnum == EWeatherEnum::EOvercast) {
		NewValue = 0.4;
	}
	else if (WeatherEnum == EWeatherEnum::ESnow) {
		NewValue = 0.2;
	}
	else  if(WeatherEnum == EWeatherEnum::ERain){
		NewValue = 0.2;
	}
	else if (WeatherEnum == EWeatherEnum::EThunder) {
		NewValue = 0.0;
	}
	else {
		NewValue = 1.0;
	}
	value = FMath::Lerp(starOpacity, NewValue, GameSpeedMultiplier / (60 * 24));
	return value;
}

EWeatherEnum ACPPGameStateBase::Weather() {
	if (GameTime[1] == 0) { //Resets temperature every hour when minute is 0 (new hour)
		if (!bHasGeneratedWeather) {
			if (bNewGenerationWeather == true) {

				int32 GeneratedWeather = FMath::RandRange(1, 4);

				if (GeneratedWeather == 1) {
					weather = EWeatherEnum::ESunny;
				}
				else if (GeneratedWeather == 2) {
					weather = EWeatherEnum::ECloudy;
				}
				else if (GeneratedWeather == 3) {
					weather = EWeatherEnum::EOvercast;
				}
				else if (GeneratedWeather == 4) {
					weather = EWeatherEnum::ERain;
				}
				else {
					weather = EWeatherEnum::ENone;
				}
				bNewGenerationWeather = false;
			}
			else if (bNewGenerationWeather == false) {
				if (GameTime[2] == 0) {
				}
				else if (LastWeather[GameTime[2] - 1] == EWeatherEnum::ESunny) {
					if (SeasonEnum == ESeasonEnum::ESpring) {
						int32 Sun = FMath::RandRange(0, 2);
						if (Sun == 0 || Sun == 1) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Sun == 2) {
							weather = EWeatherEnum::ECloudy;
						}
						else {
						weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::ESummer) {
						int32 Sun = FMath::RandRange(0, 4);
						if (Sun == 0 || Sun == 2 || Sun == 3) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Sun == 4) {
							weather = EWeatherEnum::ECloudy;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::EAutumn) {
						int32 Sun = FMath::RandRange(0, 1);
						if (Sun == 0) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Sun == 1) {
							weather = EWeatherEnum::ECloudy;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::EWinter) {
						int32 Sun = FMath::RandRange(0, 2);
						if (Sun == 0) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Sun == 1 || Sun == 2) {
							weather = EWeatherEnum::ECloudy;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
				}
				else if (LastWeather[GameTime[2] - 1] == EWeatherEnum::ECloudy) {
					if (SeasonEnum == ESeasonEnum::ESpring) {
						int32 Cloudy = FMath::RandRange(0, 7);
						if (Cloudy == 0 || Cloudy == 1) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Cloudy == 2 || Cloudy == 3 || Cloudy == 4) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Cloudy == 5 || Cloudy == 6) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Cloudy == 7) {
							weather = EWeatherEnum::EFog;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::ESummer) {
						int32 Cloudy = FMath::RandRange(0, 7);
						if (Cloudy == 0 || Cloudy == 1 || Cloudy == 2) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Cloudy == 3 || Cloudy == 4 || Cloudy == 5 || Cloudy == 6) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Cloudy == 7) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Cloudy == 8) {
							weather = EWeatherEnum::EFog;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::EAutumn) {
						int32 Cloudy = FMath::RandRange(0, 9);
						if (Cloudy == 0 || Cloudy == 1 || Cloudy == 2 || Cloudy == 3 || Cloudy == 4) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Cloudy == 5 || Cloudy == 6) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Cloudy == 7 || Cloudy == 8) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Cloudy == 9) {
							weather = EWeatherEnum::EFog;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::EWinter) {
						int32 Cloudy = FMath::RandRange(0, 8);
						if (Cloudy == 0 || Cloudy == 1 || Cloudy == 2 || Cloudy == 3) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Cloudy == 4) {
							weather = EWeatherEnum::ESunny;
						}
						else if (Cloudy == 5 || Cloudy == 6 || Cloudy == 7) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Cloudy == 8) {
							weather = EWeatherEnum::EFog;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
				}
				else if (LastWeather[GameTime[2] - 1] == EWeatherEnum::EOvercast) {
					if (SeasonEnum == ESeasonEnum::ESpring) {
						int32 Overcast = FMath::RandRange(0, 9);
						if (Overcast == 0 || Overcast == 1 || Overcast == 2) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Overcast == 3 || Overcast == 4 || Overcast == 5 || Overcast == 6) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Overcast == 7 || Overcast == 8) {
							weather = EWeatherEnum::ERain;
						}
						else if (Overcast == 9) {
							weather = EWeatherEnum::EThunder;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::ESummer) {
						int32 Overcast = FMath::RandRange(0, 5);
						if (Overcast == 0 || Overcast == 1) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Overcast == 2 || Overcast == 3 || Overcast == 4) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Overcast == 5) {
							weather = EWeatherEnum::ERain;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::EAutumn) {
						int32 Overcast = FMath::RandRange(0, 8);
						if (Overcast == 0 || Overcast == 1 || Overcast == 2) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Overcast == 3 || Overcast == 4 || Overcast == 5) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Overcast == 6 || Overcast == 7) {
							weather = EWeatherEnum::ERain;
						}
						else if (Overcast == 8) {
							weather = EWeatherEnum::EThunder;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
					else if (SeasonEnum == ESeasonEnum::EWinter) {
						int32 Overcast = FMath::RandRange(0, 9);
						if (Overcast == 0 || Overcast == 1 || Overcast == 2) {
							weather = EWeatherEnum::EOvercast;
						}
						else if (Overcast == 3 || Overcast == 4) {
							weather = EWeatherEnum::ECloudy;
						}
						else if (Overcast == 5) {
							weather = EWeatherEnum::ESnow;
						}
						else if (Overcast == 6 || Overcast == 7 || Overcast == 8) {
							weather = EWeatherEnum::ERain;
						}
						else if (Overcast == 9) {
							weather = EWeatherEnum::EThunder;
						}
						else {
							weather = EWeatherEnum::ENone;
						}
					}
				}
				else if (LastWeather[GameTime[2] - 1] == EWeatherEnum::ERain) {
					int32 Rain = FMath::RandRange(0, 8);
					if (Rain == 0 || Rain == 1 || Rain == 2 || Rain == 3) {
						weather = EWeatherEnum::ERain;
					}
					else if (Rain == 4 || Rain == 5 || Rain == 6) {
						weather = EWeatherEnum::EOvercast;
					}
					else if (Rain == 7) {
						weather = EWeatherEnum::EFog;
					}
					else if (Rain == 8) {
						weather = EWeatherEnum::EThunder;
					}
					else {
						weather = EWeatherEnum::ENone;
					}
				}
				else if (LastWeather[GameTime[2] - 1] == EWeatherEnum::EThunder) {
					int32 Thunder = FMath::RandRange(0, 6);
					if (Thunder == 0 || Thunder == 1) {
						weather = EWeatherEnum::EThunder;
					}
					else if (Thunder == 2 || Thunder == 3 || Thunder == 4) {
						weather = EWeatherEnum::ERain;
					}
					else if (Thunder == 5) {
						weather = EWeatherEnum::EOvercast;
					}
					else if (Thunder == 6) {
						weather = EWeatherEnum::ECloudy;
					}
					else {
						weather = EWeatherEnum::ENone;
					}
				}
				else if (LastWeather[GameTime[2] - 1] == EWeatherEnum::EFog) {
					int32 Fog = FMath::RandRange(0, 3);
					if (Fog == 0) {
						weather = EWeatherEnum::EFog;
					}
					else if (Fog == 1 || Fog == 2) {
						weather = EWeatherEnum::ESunny;
					}
					else if (Fog == 3) {
						weather = EWeatherEnum::ECloudy;
					}
					else {
						weather = EWeatherEnum::ENone;
					}
				}
				else if (LastWeather[GameTime[2] - 1] == EWeatherEnum::ESnow) {
					int32 Snow = FMath::RandRange(0, 3);
					if (Snow == 0 || Snow == 1) {
						weather = EWeatherEnum::ESnow;
					}
					else if (Snow == 2) {
						weather = EWeatherEnum::ECloudy;
					}
					else if (Snow == 3) {
						weather = EWeatherEnum::EOvercast;
					}
					else {
						weather = EWeatherEnum::ENone;
					}
				}
			}
			else {
				WeatherEnum = EWeatherEnum::ENone;
			}

			LastWeather.Insert(weather, GameTime[2]);
			Counter++;
			if (Counter >= 1) {
				bNewGenerationWeather = false;
			}
			bHasGeneratedWeather = true; //Makes sure generation only happens once
		}
		
	}
	else {
		bHasGeneratedWeather = false; //Resets the variable for the next hour
	}
	return weather;
}

void ACPPGameStateBase::SkyboxColour() {

	FLinearColor LocalZenith, LocalHorizon, LocalCloud, LocalOverall;

	if (WeatherEnum == EWeatherEnum::ECloudy) {
		LocalZenith = FLinearColor	(0.0f, 0.1f, 0.3f, 1.0f);
		LocalHorizon = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
		LocalCloud = FLinearColor	(0.9f, 0.9f, 1.0f, 1.0f);
		LocalOverall = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (WeatherEnum == EWeatherEnum::ESunny) {
		LocalZenith = FLinearColor	(0.1f, 0.2f, 0.3f, 1.0f);
		LocalHorizon = FLinearColor	(0.45f, 0.5f, 0.75f, 1.0f);
		LocalCloud = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
		LocalOverall = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (WeatherEnum == EWeatherEnum::EOvercast) {
		LocalZenith = FLinearColor	(0.3f, 0.4f, 0.5f, 1.0f);
		LocalHorizon = FLinearColor	(0.1f, 0.3f, 0.4f, 1.0f);
		LocalCloud = FLinearColor	(0.1f, 0.1f, 0.1f, 1.0f);
		LocalOverall = FLinearColor	(0.4f, 0.4f, 0.4f, 1.0f);
	}
	else if (WeatherEnum == EWeatherEnum::ESnow) {
		LocalZenith = FLinearColor	(0.3f, 0.4f, 0.5f, 1.0f);
		LocalHorizon = FLinearColor	(0.1f, 0.3f, 0.4f, 1.0f);
		LocalCloud = FLinearColor	(0.1f, 0.1f, 0.1f, 1.0f);
		LocalOverall = FLinearColor	(0.4f, 0.4f, 0.4f, 1.0f);
	}
	else if (WeatherEnum == EWeatherEnum::EFog) {
		LocalZenith = FLinearColor	(0.0f, 0.1f, 0.3f, 1.0f);
		LocalHorizon = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
		LocalCloud = FLinearColor	(0.9f, 0.9f, 1.0f, 1.0f);
		LocalOverall = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (WeatherEnum == EWeatherEnum::ERain) {
		LocalZenith = FLinearColor	(0.3f, 0.4f, 0.5f, 1.0f);
		LocalHorizon = FLinearColor	(0.0f, 0.2f, 0.3f, 1.0f);
		LocalCloud = FLinearColor	(0.04f, 0.04f, 0.04f, 1.0f);
		LocalOverall = FLinearColor	(0.15f, 0.15f, 0.15f, 1.0f);
	}
	else if (WeatherEnum == EWeatherEnum::EThunder) {
		LocalZenith = FLinearColor	(0.3f, 0.4f, 0.5f, 1.0f);
		LocalHorizon = FLinearColor	(0.0f, 0.2f, 0.3f, 1.0f);
		LocalCloud = FLinearColor	(0.04f, 0.04f, 0.04f, 1.0f);
		LocalOverall = FLinearColor	(0.15f, 0.15f, 0.15f, 1.0f);
	}
	else {
		LocalZenith = FLinearColor	(0.1f, 0.2f, 0.3f, 1.0f);
		LocalHorizon = FLinearColor	(0.0f, 0.6f, 1.0f, 1.0f);
		LocalCloud = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
		LocalOverall = FLinearColor	(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//if sunrise/sunset
	if (GameTime[2] == 4 || GameTime[2] == 5 || GameTime[2] == 6 ||
		GameTime[2] == 16 || GameTime[2] == 17 || GameTime[2] == 18) {
		LocalZenith = FLinearColor(0.6f, 0.1f, 0.0f, 1.0f);
		LocalHorizon = FLinearColor(1.0f, 0.6f, 0.0f, 1.0f);
		LocalCloud = FLinearColor(0.3f, 0.2f, 0.0f, 1.0f);
	}
	//if night
	if ((GameTime[2] >= 19 && GameTime[2] <= 23) || (GameTime[2] >= 0 && GameTime[2] <= 3)) {
		LocalZenith = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
		LocalHorizon = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
		LocalCloud = FLinearColor(0.04f, 0.04f, 0.04f, 1.0f);
		LocalOverall = FLinearColor(0.15f, 0.15f, 0.15f, 1.0f);
	}
	ZenithColor = FMath::Lerp(ZenithColor, LocalZenith, GameSpeedMultiplier / (60 * 24));
	HorizonColor = FMath::Lerp(HorizonColor, LocalHorizon, GameSpeedMultiplier / (60 * 24));
	CloudColor = FMath::Lerp(CloudColor, LocalCloud, GameSpeedMultiplier / (60 * 24));
	OverallColor = FMath::Lerp(OverallColor, LocalOverall, GameSpeedMultiplier / (60 * 24));
}

FString ACPPGameStateBase::FloatToDisplay(float Value, ESuffixEnum Suffix, bool bIncludeDecimal, int32 Precision) {
	//Converts float to string
	FString String = FString::SanitizeFloat(Value);

	//Finds decimal point index
	int32 DecimalPos = UKismetStringLibrary::FindSubstring(String, ".", false, false, 0);
	//Gets all numbers after decimal
	FString DecimalString = UKismetStringLibrary::GetSubstring(String, DecimalPos, 1 + Precision);
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
	else if (Suffix == ESuffixEnum::EPercent) {
		FinalString = FinalString.Append("%");
	}

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
