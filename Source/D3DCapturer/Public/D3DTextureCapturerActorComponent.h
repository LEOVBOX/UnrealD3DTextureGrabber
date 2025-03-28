// Fill out your copyright notice in the Description page of Project Settings.



#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "DXGI.lib")

#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "Components/ActorComponent.h"

#include "D3DTextureCapturerActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), DisplayName = "D3DCapturer", meta = (BlueprintSpawnableComponent))
class D3DCAPTURER_API UD3DTextureCapturerActorComponent : public UActorComponent
{
	GENERATED_BODY()

	struct CaptureContext;
	TSharedPtr<CaptureContext> context;

public:
	// Sets default values for this component's properties
	UD3DTextureCapturerActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spout")
	FName PublishName;

	// “ектура, которую будем передавать в другой процесс (из которой читаем) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spout")
	UTexture* OutputTexture;

	UTexture2D* GetCapturedTexture();
};
