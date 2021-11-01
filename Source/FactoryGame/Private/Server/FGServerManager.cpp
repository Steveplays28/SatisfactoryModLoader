// This file has been automatically generated by the Unreal Header Implementation tool

#include "Server/FGServerManager.h"

void UFGServerManager::Initialize(FSubsystemCollectionBase& Collection){ }
bool UFGServerManager::Tick(float DeltaTime){ return bool(); }
UFGServerObject* UFGServerManager::GetServerByAddress(const FString& Address, int32 QueryPort){ return nullptr; }
TScriptInterface<IFGDedicatedServerUIProxy> UFGServerManager::MakeServerInteractionHandler(){ return TScriptInterface<IFGDedicatedServerUIProxy>(); }
bool UFGServerManager::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor){ return bool(); }
void UFGServerManager::AddServer(const FString& Address, int32 Port, FOnServerAddedOrFoundDelegate OnceAdded){ }
void UFGServerManager::NativeAddServer(const FString& Address, int32 Port, FOnServerAddedOrFoundDelegateNative OnceAdded){ }
void UFGServerManager::RemoveServer(UFGServerObject* Server){ }
const TArray<class UFGServerObject*>& UFGServerManager::GetServers() const{ return *(new TArray<class UFGServerObject*>); }
void UFGServerManager::SaveState(){ }
void UFGServerManager::CustomSerialize(FArchive& Ar, EServerManagerVersion Version){ }