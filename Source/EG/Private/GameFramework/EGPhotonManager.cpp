
#include "GameFramework/EGPhotonManager.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "Templates/UniquePtr.h"

// 이제 deleter의 실제 구현을 제공합니다.
// 이 구현 위치는 PhotonClient(=Client.h)의 정의를 알고 있으므로 안전합니다.
void FPhotonClientDeleter::operator()(PhotonClient* Ptr) const
{
    if(Ptr)
    {
        delete Ptr;
    }
}

using namespace ExitGames;
using namespace ExitGames::LoadBalancing;

AEGPhotonManager::AEGPhotonManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

AEGPhotonManager::~AEGPhotonManager()
{
    // ClientOne은 자동으로 deleter를 통해 삭제됩니다.
}

void AEGPhotonManager::BeginPlay()
{
    Super::BeginPlay();

    // custom deleter를 사용하는 TUniquePtr에는 MakeUnique을 사용하지 말고 Reset(new ...) 권장
    ClientOne.Reset(new PhotonClient(*this, AppId, AppVersion));
    if (ClientOne)
        ClientOne->connect();
}

void AEGPhotonManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (ClientOne)
        ClientOne->service();
}

void AEGPhotonManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (ClientOne)
    {
        ClientOne->disconnect();
        ClientOne.Reset(); // deleter 호출되어 안전하게 delete 됨
    }
    Super::EndPlay(EndPlayReason);
}



// ================= Listener 구현 =================

void AEGPhotonManager::debugReturn(int debugLevel, const Common::JString& string)
{
    UE_LOG(LogTemp, Log, TEXT("[Photon][Dbg%d] %s"),
        debugLevel, *FString(string.UTF8Representation().cstr()));
}

void AEGPhotonManager::connectionErrorReturn(int errorCode)
{
    UE_LOG(LogTemp, Error, TEXT("[Photon] connectionErrorReturn: %d"), errorCode);
}

void AEGPhotonManager::clientErrorReturn(int errorCode)
{
    UE_LOG(LogTemp, Error, TEXT("[Photon] clientErrorReturn: %d"), errorCode);
}

void AEGPhotonManager::warningReturn(int warningCode)
{
    UE_LOG(LogTemp, Warning, TEXT("[Photon] warningReturn: %d"), warningCode);
}

void AEGPhotonManager::serverErrorReturn(int errorCode)
{
    UE_LOG(LogTemp, Error, TEXT("[Photon] serverErrorReturn: %d"), errorCode);
}

void AEGPhotonManager::joinRoomEventAction(int playerNr,
    const Common::JVector<int>& playernrs, const Player& /*player*/)
{
    UE_LOG(LogTemp, Log, TEXT("[Photon] joinRoomEventAction: playerNr=%d, playersInRoom=%d"),
        playerNr, playernrs.getSize());
}

void AEGPhotonManager::leaveRoomEventAction(int playerNr, bool isInactive)
{
    UE_LOG(LogTemp, Log, TEXT("[Photon] leaveRoomEventAction: playerNr=%d, inactive=%s"),
        playerNr, isInactive ? TEXT("true") : TEXT("false"));
}

void AEGPhotonManager::customEventAction(int playerNr, nByte eventCode,
    const Common::Object& /*eventContent*/)
{
    UE_LOG(LogTemp, Log, TEXT("[Photon] customEventAction: from=%d, code=%d"),
        playerNr, (int)eventCode);
}

void AEGPhotonManager::connectReturn(int errorCode, const Common::JString& errorString,
    const Common::JString& region, const Common::JString& cluster)
{
    if(errorCode == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("[Photon] Connected. region=%s, cluster=%s"),
            *FString(region.UTF8Representation().cstr()),
            *FString(cluster.UTF8Representation().cstr()));
        if (ClientOne)
            ClientOne->opJoinRandomRoom();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Photon] connectReturn failed: %s (code=%d)"),
            *FString(errorString.UTF8Representation().cstr()), errorCode);
    }
}

void AEGPhotonManager::disconnectReturn(void)
{
    UE_LOG(LogTemp, Log, TEXT("[Photon] Disconnected"));
}

void AEGPhotonManager::leaveRoomReturn(int errorCode, const Common::JString& errorString)
{
    if(errorCode != 0)
        UE_LOG(LogTemp, Warning, TEXT("[Photon] leaveRoomReturn: %s (code=%d)"),
            *FString(errorString.UTF8Representation().cstr()), errorCode);
}

void AEGPhotonManager::joinRandomRoomReturn(int /*localPlayerNr*/,
    const Common::Hashtable& /*roomProperties*/,
    const Common::Hashtable& /*playerProperties*/,
    int errorCode, const Common::JString& errorString)
{
    if(errorCode != 0)
    {
        UE_LOG(LogTemp, Log, TEXT("[Photon] No random room. Creating one... (%s)"),
            *FString(errorString.UTF8Representation().cstr()));
        if (ClientOne)
            ClientOne->opCreateRoom();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[Photon] joinRandomRoomReturn: success (waiting for joinRoomReturn)"));
    }
}

void AEGPhotonManager::joinRoomReturn(int /*localPlayerNr*/,
    const Common::Hashtable& /*roomProperties*/,
    const Common::Hashtable& /*playerProperties*/,
    int errorCode, const Common::JString& errorString)
{
    if(errorCode == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("[Photon] Joined Room!"));
        // 예: 이벤트 송신 샘플
        // Common::JString msg = L"Hello from UE!";
        // ClientOne->opRaiseEvent(false, msg, 1);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Photon] joinRoomReturn failed: %s (code=%d)"),
            *FString(errorString.UTF8Representation().cstr()), errorCode);
    }
}