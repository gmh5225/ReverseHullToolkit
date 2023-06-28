// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ReverseHullGenerator
{
public:
	ReverseHullGenerator();
	~ReverseHullGenerator();


	// Supports only actors with one mesh component. If the actor has more mesh components the others will be ignored
	bool GenerateHullMesh(AActor* InOrigin, bool DoCompactMesh);

	bool IsValid() const;


private:
	UMaterial* HullMaterial = nullptr;
	bool _IsValid = false;
};
