// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoadBalancing-cpp/inc/Listener.h"
#include "Templates/UniquePtr.h" // <-- TUniquePtr 선언을 헤더에서 사용하려면 필요
#include "EGPhotonManager.generated.h"

namespace ExitGames { namespace LoadBalancing { class Client; } }
using PhotonClient = ExitGames::LoadBalancing::Client;

struct FPhotonClientDeleter
{
	void operator()(PhotonClient* Ptr) const; 
};


UCLASS()
class EG_API AEGPhotonManager : public AActor, public ExitGames::LoadBalancing::Listener
{
    GENERATED_BODY()

public:
    AEGPhotonManager();
    virtual ~AEGPhotonManager();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Listener 순수가상 콜백들...
    virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override;
    virtual void connectionErrorReturn(int errorCode) override;
    virtual void clientErrorReturn(int errorCode) override;
    virtual void warningReturn(int warningCode) override;
    virtual void serverErrorReturn(int errorCode) override;

    virtual void joinRoomEventAction(int playerNr,
        const ExitGames::Common::JVector<int>& playernrs,
        const ExitGames::LoadBalancing::Player& player) override;

    virtual void leaveRoomEventAction(int playerNr, bool isInactive) override;

    virtual void customEventAction(int playerNr, nByte eventCode,
        const ExitGames::Common::Object& eventContent) override;

    // 주의: connectReturn은 4개 인자(Listener.h 기준)
    virtual void connectReturn(int errorCode,
        const ExitGames::Common::JString& errorString,
        const ExitGames::Common::JString& region,
        const ExitGames::Common::JString& cluster) override;

    virtual void disconnectReturn(void) override;

    virtual void leaveRoomReturn(int errorCode,
        const ExitGames::Common::JString& errorString) override;

    // 선택 콜백(기본 구현 존재) — 흐름 파악을 위해 override
    virtual void joinRandomRoomReturn(int /*localPlayerNr*/,
        const ExitGames::Common::Hashtable& /*roomProperties*/,
        const ExitGames::Common::Hashtable& /*playerProperties*/,
        int /*errorCode*/,
        const ExitGames::Common::JString& /*errorString*/) override;

    virtual void joinRoomReturn(int /*localPlayerNr*/,
        const ExitGames::Common::Hashtable& /*roomProperties*/,
        const ExitGames::Common::Hashtable& /*playerProperties*/,
        int /*errorCode*/,
        const ExitGames::Common::JString& /*errorString*/) override;

    
private:
    TUniquePtr<PhotonClient, FPhotonClientDeleter> ClientOne;

    ExitGames::Common::JString AppId      = L"fda07087-43ac-4fd5-b80c-0503ba30b29c";
    ExitGames::Common::JString AppVersion = L"1.0";
};