// Fill out your copyright notice in the Description page of Project Settings.

#include "D3DTextureCapturerActorComponent.h"
#include<windows.h>

#include <string>
#include <map>

#include "Windows/AllowWindowsPlatformTypes.h" 
#include <d3d11on12.h>
#include "Windows/HideWindowsPlatformTypes.h"

static std::map<std::string, int> sender_name_reference_countor;

struct UD3DTextureCapturerActorComponent::CaptureContext
{
	ID3D11Device* D3D11Device = nullptr;
	ID3D11On12Device* D3D11on12Device = nullptr;
	ID3D11Resource* WrappedDX11Resource = nullptr;

	FName Name;
	std::string Name_str;
	unsigned int width = 0, height = 0;

	HANDLE sharedSendingHandle = nullptr;
	ID3D11Texture2D* sendingTexture = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	FRHITexture2D* Texture2D = nullptr;

	CaptureContext(const FName& Name,
		FRHITexture2D* Texture2D)
		: Name(Name)
		, Texture2D(Texture2D)
	{
		UE_LOG(LogTemp, Warning, TEXT("CaptureContext"));

		DXGI_FORMAT texFormat;
		FString RHIName = GDynamicRHI->GetName();

		if (RHIName == TEXT("D3D11"))
		{
			D3D11Device = static_cast<ID3D11Device*>(GDynamicRHI->RHIGetNativeDevice());
			D3D11Device->GetImmediateContext(&deviceContext);

			ID3D11Texture2D* NativeTex = (ID3D11Texture2D*)Texture2D->GetNativeResource();

			D3D11_TEXTURE2D_DESC desc;
			NativeTex->GetDesc(&desc);

			width = desc.Width;
			height = desc.Height;

			texFormat = desc.Format;
		}
		else if (RHIName == TEXT("D3D12"))
		{
			ID3D12Device* Device12 = static_cast<ID3D12Device*>(GDynamicRHI->RHIGetNativeDevice());
			if (Device12 == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Device12 = nullptr"));
			}

			UINT DeviceFlags11 = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

			verify(D3D11On12CreateDevice(
				Device12,
				DeviceFlags11,
				nullptr,
				0,
				nullptr,
				0,
				0,
				&D3D11Device,
				&deviceContext,
				nullptr
			) == S_OK);

			verify(D3D11Device->QueryInterface(__uuidof(ID3D11On12Device), (void**)&D3D11on12Device) == S_OK);

			D3D12_RESOURCE_DESC desc;
			ID3D12Resource* NativeTex = (ID3D12Resource*)Texture2D->GetNativeResource();
			desc = NativeTex->GetDesc();

			width = desc.Width;
			height = desc.Height;

			texFormat = desc.Format;

			D3D11_RESOURCE_FLAGS rf11 = {};

			verify(D3D11on12Device->CreateWrappedResource(
				NativeTex, &rf11,
				D3D12_RESOURCE_STATE_COPY_SOURCE,
				D3D12_RESOURCE_STATE_PRESENT, __uuidof(ID3D11Resource),
				(void**)&WrappedDX11Resource) == S_OK);

		}
		else
		{
			throw;
		}

		if (texFormat == DXGI_FORMAT_B8G8R8A8_TYPELESS) {
			texFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
		}

		// Создаем текстуру для копирования (sendingTexture)
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_STAGING; // Для чтения на CPU (если нужно в GetCapturedTexture)
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // Разрешаем чтение
		desc.MiscFlags = 0;

		// Создаем текстуру
		HRESULT hr = D3D11Device->CreateTexture2D(&desc, nullptr, &sendingTexture);
		if (FAILED(hr))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create sendingTexture!"));
			sendingTexture = nullptr;
		}
	}

	~CaptureContext()
	{
		if (sendingTexture)
		{
			sendingTexture->Release();
			sendingTexture = nullptr;
		}

		if (deviceContext)
		{
			deviceContext->Flush();
			deviceContext->Release();
			deviceContext = nullptr;
		}

		if (WrappedDX11Resource)
		{
			D3D11on12Device->ReleaseWrappedResources(&WrappedDX11Resource, 1);
			WrappedDX11Resource = nullptr;
		}

		if (D3D11on12Device)
		{
			D3D11on12Device->Release();
			D3D11on12Device = nullptr;
		}

		if (D3D11Device)
		{
			D3D11Device->Release();
			D3D11Device = nullptr;
		}
	}

	void Tick()
	{
		//UE_LOG(LogTemp, Warning, TEXT("D3DTextureCapturer::Tick()"));
		if (!deviceContext)
		{
			UE_LOG(LogTemp, Error, TEXT("D3DTextureCapturer::Tick() !deviceContext"));
			return;
		}
			

		FString RHIName = GDynamicRHI->GetName();

		if (RHIName == TEXT("D3D11"))
		{
			ENQUEUE_RENDER_COMMAND(D3DTextureCapturerCommand)([this](FRHICommandListImmediate& RHICmdList) {
				ID3D11Texture2D* NativeTex = (ID3D11Texture2D*)Texture2D->GetNativeResource();

				this->deviceContext->CopyResource(sendingTexture, NativeTex);
				this->deviceContext->Flush();
				});
		}
		else if (RHIName == TEXT("D3D12"))
		{
			ENQUEUE_RENDER_COMMAND(D3DTextureCapturerCommand)([this](FRHICommandListImmediate& RHICmdList) {
				this->D3D11on12Device->AcquireWrappedResources(&WrappedDX11Resource, 1);
				this->deviceContext->CopyResource(sendingTexture, WrappedDX11Resource);
				this->D3D11on12Device->ReleaseWrappedResources(&WrappedDX11Resource, 1);
				this->deviceContext->Flush();
				});
		}
	}

	const FName& GetName() const { return Name; }

};

///////////////////////////////////////////////////////////////////////////////

UD3DTextureCapturerActorComponent::UD3DTextureCapturerActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
}

void UD3DTextureCapturerActorComponent::BeginPlay()
{
	Super::BeginPlay();

	context.Reset();
}

void UD3DTextureCapturerActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	context.Reset();

	Super::EndPlay(EndPlayReason);
}

void UD3DTextureCapturerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OutputTexture
		|| !OutputTexture->GetResource()->TextureRHI) return;

	auto Texture2D = OutputTexture->GetResource()->TextureRHI->GetTexture2D();
	if (!Texture2D)
	{
		context.Reset();
		return;
	}

	if (!Texture2D->GetNativeResource())
	{
		context.Reset();
		return;
	}

	if (!context.IsValid())
	{
		context = TSharedPtr<CaptureContext>(new CaptureContext(PublishName, Texture2D));
	}
	else if (PublishName != context->GetName())
	{
		context.Reset();
		return;
	}

	ID3D12Device* Device12 = static_cast<ID3D12Device*>(GDynamicRHI->RHIGetNativeDevice());
	HRESULT hr = Device12->GetDeviceRemovedReason();
	if (hr != S_OK)
	{
		UE_LOG(LogTemp, Error, TEXT("D3D12 device in removed state! Cannot proceed."));
		context.Reset();
		return;
	}

	context->Tick();
}

// ВЫЗЫВАЕТ ОШИБКУ GPU DEVICE REMOVED
UTexture2D* UD3DTextureCapturerActorComponent::GetCapturedTexture()
{
	if (!context.IsValid() || !context->sendingTexture)
	{
		UE_LOG(LogTemp, Error, TEXT("UD3DTextureCapturerActorComponent::GetCapturedTexture() !context.IsValid() || !context->sendingTexture"));
		return nullptr;
	}

	// Получаем описание текстуры
	D3D11_TEXTURE2D_DESC desc;
	context->sendingTexture->GetDesc(&desc);


	if (desc.Usage != D3D11_USAGE_STAGING) {
		UE_LOG(LogTemp, Error, TEXT("context->sendingTexture is not a STAGING texture!"));
		return nullptr;
	}

	// Создаем новую UTexture2D
	UTexture2D* CapturedTexture = UTexture2D::CreateTransient(desc.Width, desc.Height, PF_B8G8R8A8);
	if (!CapturedTexture)
	{
		return nullptr;
	}

	CapturedTexture->AddToRoot(); // Предотвращает сборку мусора

	// Блокируем текстуру в которую пишем
	FTexture2DMipMap& Mip = CapturedTexture->GetPlatformData()->Mips[0];
	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);

	// Копируем данные из D3D11 текстуры в буфер UE
	ID3D11DeviceContext* DeviceContext = context->deviceContext;
	if (DeviceContext)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(GetCapturedTexture Flush);
		double StartTime = FPlatformTime::Seconds();

		TRACE_CPUPROFILER_EVENT_SCOPE(GetCapturedTexture Flush)
		DeviceContext->Flush();

		double EndTime = FPlatformTime::Seconds();
		UE_LOG(LogTemp, Log, TEXT("Flush took: %f ms"), (EndTime - StartTime) * 1000.0);
		

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		TRACE_CPUPROFILER_EVENT_SCOPE(GetCapturedTexture Map)
		HRESULT hr = DeviceContext->Map(context->sendingTexture, 0, D3D11_MAP_READ, 0, &MappedResource);
		if (SUCCEEDED(hr)) {
			TRACE_CPUPROFILER_EVENT_SCOPE(GetCapturedTexture Memcpy)
			FMemory::Memcpy(TextureData, MappedResource.pData, desc.Width * desc.Height * 4); // RGBA8 (4 байта на пиксель)
			DeviceContext->Unmap(context->sendingTexture, 0);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Map() failed! HRESULT: %d"), hr);
			
			/*if (!OutputTexture
				|| !OutputTexture->GetResource()->TextureRHI) return nullptr;

			auto Texture2D = OutputTexture->GetResource()->TextureRHI->GetTexture2D();
			context.Reset();
			UE_LOG(LogTemp, Error, TEXT("D3DTextureCapturer context resetted"));*/
			return nullptr;
		}

		//auto pslice = reinterpret_cast<const uint8*>(MappedResource.pData);
		//if (!pslice)
		//{
		//	DeviceContext->Unmap(context->WrappedDX11Resource, 0);
		//	UE_LOG(LogTemp, Error, TEXT("DeviceContext->Unmap(context->WrappedDX11Resource, 0)"));
		//}

	}

	Mip.BulkData.Unlock();

	CapturedTexture->UpdateResource();

	return CapturedTexture;
}


//UTexture2D* UD3DTextureCapturerActorComponent::GetCapturedTexture()
//{
//	if (!context.IsValid() || !context->sendingTexture)
//	{
//		UE_LOG(LogTemp, Error, TEXT("UD3DTextureCapturerActorComponent::GetCapturedTexture() !context.IsValid() || !context->sendingTexture"));
//		return nullptr;
//	}
//
//	// Получаем описание текстуры
//	D3D11_TEXTURE2D_DESC desc;
//	context->sendingTexture->GetDesc(&desc);
//
//
//	if (desc.Usage != D3D11_USAGE_STAGING) {
//		UE_LOG(LogTemp, Error, TEXT("context->sendingTexture is not a STAGING texture!"));
//		return nullptr;
//	}
//
//	// Создаем новую UTexture2D
//	UTexture2D* CapturedTexture = UTexture2D::CreateTransient(desc.Width, desc.Height, PF_B8G8R8A8);
//	if (!CapturedTexture)
//	{
//		return nullptr;
//	}
//
//	CapturedTexture->AddToRoot(); // Предотвращает сборку мусора
//
//	// Блокируем текстуру в которую пишем
//	FTexture2DMipMap& Mip = CapturedTexture->GetPlatformData()->Mips[0];
//	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
//	if (desc.Usage != D3D11_USAGE_STAGING) {
//		UE_LOG(LogTemp, Error, TEXT("CaptureTextureOnRenderThread: Texture is not a STAGING texture!"));
//		return nullptr;
//	}
//
//	ID3D11DeviceContext* DeviceContext = context->deviceContext;
//	if (!DeviceContext) {
//		UE_LOG(LogTemp, Error, TEXT("CaptureTextureOnRenderThread: DeviceContext is null"));
//		return nullptr;
//	}
//
//	// Выполняем Map() в Render Thread
//	ENQUEUE_RENDER_COMMAND(CaptureTexture)(
//		[this, DeviceContext, TextureData, desc](FRHICommandListImmediate& RHICmdList)
//		{
//			TRACE_CPUPROFILER_EVENT_SCOPE(GetCapturedTexture Map)
//			D3D11_MAPPED_SUBRESOURCE MappedResource;
//			HRESULT hr = DeviceContext->Map(context->sendingTexture, 0, D3D11_MAP_READ, 0, &MappedResource);
//			if (SUCCEEDED(hr)) 
//			{
//				TRACE_CPUPROFILER_EVENT_SCOPE(GetCapturedTexture Map)
//				// Копируем данные из D3D текстуры в TargetTexture
//				FMemory::Memcpy(TextureData, MappedResource.pData, desc.Width * desc.Height * 4); // RGBA8 (4 байта на пиксель)
//
//				DeviceContext->Unmap(context->sendingTexture, 0);
//				return;
//			}
//			else 
//			{
//				UE_LOG(LogTemp, Error, TEXT("CaptureTextureOnRenderThread: Map() failed! HRESULT: %d"), hr);
//				return;
//			}
//
//			//DeviceContext->Flush();
//		}
//	);
//
//	Mip.BulkData.Unlock();
//
//	return CapturedTexture;
//}