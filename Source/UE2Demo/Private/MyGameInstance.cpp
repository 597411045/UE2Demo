// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "IPlatformFilePak.h"
#include "Kismet/KismetStringLibrary.h"

UObject* UMyGameInstance::MountPaks(const FString& path)
{
	// const FString& path2 = "D:\\CusPak\\TestPak.pak";
	//
	//
	// IPlatformFile* InnerPlatformFile = &FPlatformFileManager::Get().GetPlatformFile();
	// //初始化PakPlatformFile
	// FPakPlatformFile* PakPlatformFile = new FPakPlatformFile();
	// PakPlatformFile->Initialize(InnerPlatformFile, TEXT(""));
	// // 切换到Pak平台
	// FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
	//
	//
	// const TRefCountPtr<FPakFile> TmpPak = new FPakFile(InnerPlatformFile, *path2, false);
	//
	// TmpPak->SetMountPoint(*FPaths::ProjectContentDir());
	//
	// if (PakPlatformFile->Mount(*path2, 1, *TmpPak->GetMountPoint()))
	// {
	// 	TArray<FString> FoundFilenames;
	// 	TmpPak->FindPrunedFilesAtPath(FoundFilenames, *TmpPak->GetMountPoint(), true, false, true);
	//
	// 	for (const auto& FileName : FoundFilenames)
	// 	{
	// 		//if (FileName.EndsWith(TEXT(".uasset")))
	// 		{
	UObject* tmp = StaticLoadObject(UObject::StaticClass(), NULL, *path);
	if (tmp)
	{
		//FPlatformFileManager::Get().SetPlatformFile(*OldPlatform);
		return tmp;
	}
	// 		}
	// 	}
	// }

	// FString MountPoint = PakFile->GetMountPoint();
	// //按需调整挂载点，若Pak包里包含正确挂载点，则无需调整
	// //PIE模式
	// //MountPoint = FPaths::ProjectContentDir() + TEXT("Pak1/");
	// //打包模式
	// //MountPoint = TEXT("../../../UE2Demo/Content/CusPak/test01.pak");
	// //对pak文件进行挂载
	// //100000为优先级
	// //若Pak包里自带正确挂载点，*MountPoint可不传
	//FPlatformFileManager::Get().SetPlatformFile(*OldPlatform);
	return nullptr;
}
