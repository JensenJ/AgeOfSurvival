// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPInventoryItem.h"

// Sets default values
FInventoryItem::FInventoryItem()
{
	this->Name = FText::FromString("No Name");
	this->Weight = 1;
	this->Value = 1;
	this->Description = FText::FromString("No Description");
}