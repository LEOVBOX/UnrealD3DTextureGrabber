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
    ID3D11Texture2D* sendingTextures[2] = { nullptr, nullptr }; // Двойная буферизация
    int currentTextureIndex = 0; // Индекс текущей текстуры для записи
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

        // Создаем две текстуры для двойной буферизации
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.BindFlags = 0;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.MiscFlags = 0;

        for (int i = 0; i < 2; i++)
        {
            HRESULT hr = D3D11Device->CreateTexture2D(&desc, nullptr, &sendingTextures[i]);
            if (FAILED(hr))
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create sendingTexture %d!"), i);
                sendingTextures[i] = nullptr;
            }
        }
    }

    ~CaptureContext()
    {
        for (int i = 0; i < 2; i++)
        {
            if (sendingTextures[i])
            {
                sendingTextures[i]->Release();
                sendingTextures[i] = nullptr;
            }
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
        if (!deviceContext)
        {
            UE_LOG(LogTemp, Error, TEXT("D3DTextureCapturer::Tick() !deviceContext"));
            return;
        }

        // Переключаем индекс текущей текстуры
        currentTextureIndex = (currentTextureIndex + 1) % 2;
        ID3D11Texture2D* currentSendingTexture = sendingTextures[currentTextureIndex];

        FString RHIName = GDynamicRHI->GetName();

        if (RHIName == TEXT("D3D11"))
        {
            ENQUEUE_RENDER_COMMAND(D3DTextureCapturerCommand)([this, currentSendingTexture](FRHICommandListImmediate& RHICmdList) {
                ID3D11Texture2D* NativeTex = (ID3D11Texture2D*)Texture2D->GetNativeResource();
                this->deviceContext->CopyResource(currentSendingTexture, NativeTex);
                this->deviceContext->Flush();
                });
        }
        else if (RHIName == TEXT("D3D12"))
        {
            ENQUEUE_RENDER_COMMAND(D3DTextureCapturerCommand)([this, currentSendingTexture](FRHICommandListImmediate& RHICmdList) {
                this->D3D11on12Device->AcquireWrappedResources(&WrappedDX11Resource, 1);
                this->deviceContext->CopyResource(currentSendingTexture, WrappedDX11Resource);
                this->D3D11on12Device->ReleaseWrappedResources(&WrappedDX11Resource, 1);
                this->deviceContext->Flush();
                });
        }
    }

    // Получить текущую текстуру для чтения (другая, чем та, в которую идет запись)
    ID3D11Texture2D* GetReadTexture() const
    {
        return sendingTextures[(currentTextureIndex + 1) % 2];
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

bool UD3DTextureCapturerActorComponent::GetCurrentFramePixels(TArray<FColor>& OutPixels)
{
    if (!context.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Capture context not initialized"));
        return false;
    }

    ID3D11Texture2D* readTexture = context->GetReadTexture();
    if (!readTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("No texture available for reading"));
        return false;
    }

    D3D11_TEXTURE2D_DESC desc;
    readTexture->GetDesc(&desc);

    // Проверяем формат
    if (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unsupported texture format"));
        return false;
    }

    // Подготавливаем выходной массив
    OutPixels.SetNumUninitialized(desc.Width * desc.Height);

    // Копируем данные
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = context->deviceContext->Map(readTexture, 0, D3D11_MAP_READ, 0, &mapped);
    if (FAILED(hr))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to map texture (HRESULT: %08X)"), hr);
        return false;
    }

    // Копируем с учетом pitch (выравнивания строк)
    const uint8* src = (uint8*)mapped.pData;
    FColor* dst = OutPixels.GetData();
    const uint32 rowSize = desc.Width * sizeof(FColor);

    for (uint32 y = 0; y < desc.Height; y++)
    {
        FMemory::Memcpy(dst, src, rowSize);
        src += mapped.RowPitch;
        dst += desc.Width;
    }

    context->deviceContext->Unmap(readTexture, 0);
    return true;
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

bool UD3DTextureCapturerActorComponent::GetCapturedTexture(UTexture2D*& outputTexture)
{
    if (!context.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("UD3DTextureCapturerActorComponent::GetCapturedTexture() !context.IsValid()"));
        return false;
    }

    ID3D11Texture2D* readTexture = context->GetReadTexture();
    if (!readTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("readTexture is null!"));
        return false;
    }

    // Получаем описание текстуры
    D3D11_TEXTURE2D_DESC desc;
    readTexture->GetDesc(&desc);

    if (desc.Usage != D3D11_USAGE_STAGING) {
        UE_LOG(LogTemp, Error, TEXT("readTexture is not a STAGING texture!"));
        return false;
    }

    // Создаем новую UTexture2D только если изменились размеры текстуры
    if (!outputTexture || desc.Width != outputTexture->GetSizeX() || desc.Height != outputTexture->GetSizeY())
    {
        UTexture2D* NewTexture = UTexture2D::CreateTransient(desc.Width, desc.Height, PF_B8G8R8A8);
        if (!NewTexture)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create UTexture2D!"));
            return false;
        }

        if (outputTexture)
        {
            outputTexture->RemoveFromRoot();
            outputTexture->ConditionalBeginDestroy();
        }

        outputTexture = NewTexture;
        outputTexture->AddToRoot();
    }

    // Блокируем текстуру в которую пишем
    FTexture2DMipMap& Mip = outputTexture->GetPlatformData()->Mips[0];

    ID3D11DeviceContext* DeviceContext = context->deviceContext;
    if (!DeviceContext)
    {
        UE_LOG(LogTemp, Error, TEXT("DeviceContext is null!"));
        return false;
    }

    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
    if (!TextureData)
    {
        UE_LOG(LogTemp, Error, TEXT("Mip.BulkData.Lock() failed!"));
        return false;
    }

    //DeviceContext->Flush();

    D3D11_MAPPED_SUBRESOURCE MappedResource;
    HRESULT hr = DeviceContext->Map(readTexture, 0, D3D11_MAP_READ, 0, &MappedResource);
    if (FAILED(hr))
    {
        UE_LOG(LogTemp, Error, TEXT("Map() failed! HRESULT: %d"), hr);
        Mip.BulkData.Unlock();
        return false;
    }

    FMemory::Memcpy(TextureData, MappedResource.pData, desc.Width * desc.Height * 4);
    DeviceContext->Unmap(readTexture, 0);

    Mip.BulkData.Unlock();
    outputTexture->UpdateResource();

    return true;
}