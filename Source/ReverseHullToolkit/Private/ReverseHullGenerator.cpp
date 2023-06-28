// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Editor.h"
#include "GeometryScript/MeshRepairFunctions.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "UDynamicMesh.h"
#include "Components/StaticMeshComponent.h"
#include "ReverseHullGenerator.h"

ReverseHullGenerator::ReverseHullGenerator()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FSoftObjectPath material_path;
	material_path.SetPath("/ReverseHullToolkit/M_ReverseHull.M_ReverseHull");
	FAssetData hull_material = AssetRegistryModule.Get().GetAssetByObjectPath(material_path);
	if (hull_material.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("hull material found!"));
	}
	UObject* mat_asset = hull_material.GetAsset();

	HullMaterial = Cast<UMaterial>(mat_asset);
	if (HullMaterial)
	{
		UE_LOG(LogTemp, Display, TEXT("hull material saved in generator!"));
		_IsValid = true;
	}
}

bool ReverseHullGenerator::GenerateHullMesh(AActor* InOrigin, bool DoCompactMesh)
{
	UStaticMeshComponent* original_mesh = Cast<UStaticMeshComponent>(InOrigin->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	if (!original_mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("The selected actor does not have a Static Mesh Component"));
		return false;
	}
	
	FString name = InOrigin->GetActorLabel();
	name.Append("Outline");

	AActor* OutGenerated = GEditor->GetEditorWorldContext().World()->SpawnActor<AActor>(InOrigin->GetClass(), InOrigin->GetActorTransform());
	OutGenerated->AttachToActor(InOrigin, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OutGenerated->SetActorLabel(name);

	if (!DoCompactMesh)
	{
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(OutGenerated->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		mesh->SetStaticMesh(original_mesh->GetStaticMesh());
		mesh->SetMaterial(0, HullMaterial);
	}
	else
	{
		// placeholder
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(OutGenerated->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		mesh->SetStaticMesh(original_mesh->GetStaticMesh());
		mesh->SetMaterial(0, HullMaterial);

		UE_LOG(LogTemp, Warning, TEXT("Compact mesh funcionality is not yet implemented!"));
		return false;
	}

	return true;
}

bool ReverseHullGenerator::IsValid() const
{
	return _IsValid;
}

ReverseHullGenerator::~ReverseHullGenerator()
{
}
