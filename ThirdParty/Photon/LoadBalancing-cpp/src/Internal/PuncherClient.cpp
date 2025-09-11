/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2025 Exit Games GmbH. All rights reserved.
 * https://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/Internal/PuncherClient.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/EventCode.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			using namespace Common;
			using namespace Common::MemoryManagement;
			using namespace Internal::EventCode;
			using namespace Photon::Punchthrough;

			PuncherClient::PuncherClient(Client& client, Listener& listener, const Common::Logger& logger, nByte serializationProtocol)
				: mpPuncher(NULL)
				, mLoadBalancingClient(client)
				, mLoadBalancingListener(listener)
				, mLogger(logger)
				, M_SERIALIZATION_PROTOCOL(serializationProtocol)
			{
			}

			PuncherClient::~PuncherClient(void)
			{
				deallocate(mpPuncher);
			}

			bool PuncherClient::initPuncher(void)
			{
				ALLOCATE(Puncher, mpPuncher, this, mLogger);
				if(mpPuncher->init(this))
					EGLOG(DebugLevel::INFO, L"Puncher init OK");
				else
					EGLOG(DebugLevel::ERRORS, L"Failed to init Puncher");
				return true;
			}

			bool PuncherClient::startPunch(int playerNr)
			{
				if(mpPuncher)
				{
					bool res = mpPuncher->startPunch(playerNr);
					if(res)
						EGLOG(DebugLevel::ALL, L"punch started with player %d", playerNr);
					else
						EGLOG(DebugLevel::ERRORS, L"punch start failed with player %d", playerNr);
					return res;
				}
				else
				{
					EGLOG(DebugLevel::ERRORS, L"startPunch: puncher is not initialized");
					return false;
				}
			}

			bool PuncherClient::sendDirect(const JVector<nByte>& buffer, int targetID, bool fallbackRelay)
			{
				if(mpPuncher)
					return mpPuncher->sendDirect(buffer, targetID, fallbackRelay);
				else
				{
					EGLOG(DebugLevel::ERRORS, L" puncher is not initialized");
					return false;
				}
			}

			int PuncherClient::sendDirect(const JVector<nByte>& buffer, const JVector<int>& targetIDs, bool fallbackRelay)
			{
				if(mpPuncher)
					return mpPuncher->sendDirect(buffer, targetIDs, fallbackRelay);
				else
				{
					EGLOG(DebugLevel::ERRORS, L"puncher is not initialized");
					return 0;
				}
			}

			void PuncherClient::service(void)
			{
				if(mpPuncher)
					mpPuncher->service();
			}

			bool PuncherClient::processRelayPackage(const JVector<nByte>& packet, int remoteID)
			{
				if(mpPuncher)
					return mpPuncher->processRelayPackage(packet, remoteID);
				else
					return false;
			}

			int PuncherClient::getLocalID(void)
			{
				return mLoadBalancingClient.getLocalPlayer().getNumber();
			}

			void PuncherClient::onDirectConnectionEstablished(int remoteID)
			{
				mLoadBalancingListener.onDirectConnectionEstablished(remoteID);
			}

			void PuncherClient::onDirectConnectionFailedToEstablish(int remoteID)
			{
				mLoadBalancingListener.onDirectConnectionFailedToEstablish(remoteID);
			}

			void PuncherClient::onReceiveDirect(const JVector<nByte>& buffer, int remoteID, bool relay) 
			{
				Deserializer d(buffer.getCArray(), buffer.getSize(), M_SERIALIZATION_PROTOCOL);
				Object msg;
				d.pop(msg);
				mLoadBalancingListener.onDirectMessage(msg, remoteID, relay);
			}

			bool PuncherClient::sendRelay(bool reliable, const JVector<nByte>& buffer, const JVector<int>& targetIDs)
			{
				return buffer.getSize()?mLoadBalancingClient.opRaiseEvent(reliable, buffer.getCArray(), buffer.getSize(), PUNCH_MSG, RaiseEventOptions().setTargetPlayers(targetIDs.getCArray(), static_cast<int>(targetIDs.getSize()))):false;
			}
		}
	}
}
