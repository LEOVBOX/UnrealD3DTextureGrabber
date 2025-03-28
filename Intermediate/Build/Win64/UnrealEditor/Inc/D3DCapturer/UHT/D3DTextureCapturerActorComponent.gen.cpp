// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "D3DCapturer/Public/D3DTextureCapturerActorComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeD3DTextureCapturerActorComponent() {}
// Cross Module References
	D3DCAPTURER_API UClass* Z_Construct_UClass_UD3DTextureCapturerActorComponent();
	D3DCAPTURER_API UClass* Z_Construct_UClass_UD3DTextureCapturerActorComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	ENGINE_API UClass* Z_Construct_UClass_UTexture_NoRegister();
	UPackage* Z_Construct_UPackage__Script_D3DCapturer();
// End Cross Module References
	void UD3DTextureCapturerActorComponent::StaticRegisterNativesUD3DTextureCapturerActorComponent()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UD3DTextureCapturerActorComponent);
	UClass* Z_Construct_UClass_UD3DTextureCapturerActorComponent_NoRegister()
	{
		return UD3DTextureCapturerActorComponent::StaticClass();
	}
	struct Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PublishName_MetaData[];
#endif
		static const UECodeGen_Private::FNamePropertyParams NewProp_PublishName;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutputTexture_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutputTexture;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_D3DCapturer,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "DisplayName", "D3DCapturer" },
		{ "IncludePath", "D3DTextureCapturerActorComponent.h" },
		{ "ModuleRelativePath", "Public/D3DTextureCapturerActorComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_PublishName_MetaData[] = {
		{ "Category", "Spout" },
		{ "ModuleRelativePath", "Public/D3DTextureCapturerActorComponent.h" },
	};
#endif
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_PublishName = { "PublishName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UD3DTextureCapturerActorComponent, PublishName), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_PublishName_MetaData), Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_PublishName_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_OutputTexture_MetaData[] = {
		{ "Category", "Spout" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd, \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd (\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd) \n" },
#endif
		{ "ModuleRelativePath", "Public/D3DTextureCapturerActorComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd, \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd (\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd)" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_OutputTexture = { "OutputTexture", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UD3DTextureCapturerActorComponent, OutputTexture), Z_Construct_UClass_UTexture_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_OutputTexture_MetaData), Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_OutputTexture_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_PublishName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::NewProp_OutputTexture,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UD3DTextureCapturerActorComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::ClassParams = {
		&UD3DTextureCapturerActorComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UD3DTextureCapturerActorComponent()
	{
		if (!Z_Registration_Info_UClass_UD3DTextureCapturerActorComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UD3DTextureCapturerActorComponent.OuterSingleton, Z_Construct_UClass_UD3DTextureCapturerActorComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UD3DTextureCapturerActorComponent.OuterSingleton;
	}
	template<> D3DCAPTURER_API UClass* StaticClass<UD3DTextureCapturerActorComponent>()
	{
		return UD3DTextureCapturerActorComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UD3DTextureCapturerActorComponent);
	UD3DTextureCapturerActorComponent::~UD3DTextureCapturerActorComponent() {}
	struct Z_CompiledInDeferFile_FID_Users_leovb_Documents_Unreal_Projects_Sandbox_Plugins_D3DCapturer_Source_D3DCapturer_Public_D3DTextureCapturerActorComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_leovb_Documents_Unreal_Projects_Sandbox_Plugins_D3DCapturer_Source_D3DCapturer_Public_D3DTextureCapturerActorComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UD3DTextureCapturerActorComponent, UD3DTextureCapturerActorComponent::StaticClass, TEXT("UD3DTextureCapturerActorComponent"), &Z_Registration_Info_UClass_UD3DTextureCapturerActorComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UD3DTextureCapturerActorComponent), 714148341U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_leovb_Documents_Unreal_Projects_Sandbox_Plugins_D3DCapturer_Source_D3DCapturer_Public_D3DTextureCapturerActorComponent_h_3187023816(TEXT("/Script/D3DCapturer"),
		Z_CompiledInDeferFile_FID_Users_leovb_Documents_Unreal_Projects_Sandbox_Plugins_D3DCapturer_Source_D3DCapturer_Public_D3DTextureCapturerActorComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_leovb_Documents_Unreal_Projects_Sandbox_Plugins_D3DCapturer_Source_D3DCapturer_Public_D3DTextureCapturerActorComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
