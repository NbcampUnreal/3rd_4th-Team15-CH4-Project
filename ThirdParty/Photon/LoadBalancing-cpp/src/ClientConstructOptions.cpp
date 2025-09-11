/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2025 Exit Games GmbH. All rights reserved.
 * https://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/ClientConstructOptions.h"

/** @file LoadBalancing-cpp/inc/ClientConstructOptions.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		
		/** @class ClientConstructOptions
		   This class aggregates the various optional parameters that can be passed to the constructor of class Client.
		   @sa Client::connect() */
		
		/**
		   Constructor: Creates a new instance with the specified parameters.
		   @param connectionProtocol The protocol to use to connect to the %Photon servers. Must match one of the constants specified in ConnectionProtocol. 
		   @param autoLobbyStats Pass true, if you want to automatically receive updates for the lobby stats, false otherwise. Call opLobbyStats() to explicitly request a lobby stats update.
		   @param regionSelectionMode Determines how the %Photon Cloud Region to which the Client connects should be selected. Must match one of the constants specified in RegionSelectionMode.
		   This parameter gets ignored when connecting to %Photon Server.
		   @param useAlternativePorts Determines if the the standard or the alternative port range should be used. This parameter currently is only relevant when ConnectionProtocol::UDP is passed
		   for parameter connectionProtocol and gets ignored otherwise. A router or firewall might block connections that use one port-range but don't block connections that use the other, so when
		   connecting with one range fails, then you may want to try with the other one.
		   @param serializationProtocol The protocol to use for serialization and deserialization of data. Must match one of the constants specified in SerializationProtocol. */
		ClientConstructOptions::ClientConstructOptions(nByte connectionProtocol, bool autoLobbyStats, nByte regionSelectionMode, bool useAlternativePorts, nByte serializationProtocol)
			: mConnectionProtocol(connectionProtocol)
			, mAutoLobbyStats(autoLobbyStats)
			, mRegionSelectionMode(regionSelectionMode)
			, mUseAlternativePorts(useAlternativePorts)
			, mSerializationProtocol(serializationProtocol)
		{
		}
		
		/**
		   Destructor. */
		ClientConstructOptions::~ClientConstructOptions(void)
		{
		}
		
		/**
		   Copy-Constructor: Creates a new instance that is a deep copy of the argument instance.
		   @param toCopy The instance to copy. */
		ClientConstructOptions::ClientConstructOptions(const ClientConstructOptions& toCopy)
		{
			*this = toCopy;
		}
		
		/**
		   operator=.
	   
		   Makes a deep copy of its right operand into its left operand.
	   
		   This overwrites old data in the left operand. */
		ClientConstructOptions& ClientConstructOptions::operator=(const ClientConstructOptions& toCopy)
		{
			mConnectionProtocol = toCopy.mConnectionProtocol;
			mAutoLobbyStats = toCopy.mAutoLobbyStats;
			mRegionSelectionMode = toCopy.mRegionSelectionMode;
			mUseAlternativePorts = toCopy.mUseAlternativePorts;
			mSerializationProtocol = toCopy.mSerializationProtocol;

			return *this;
		}

		/**
		   @returns the currently set ConnectionProtocol
		   @sa
		   setConnectionProtocol(), ConnectionProtocol */
		nByte ClientConstructOptions::getConnectionProtocol(void) const
		{
			return mConnectionProtocol;
		}
		
		/**
		   Sets the ConnectionProtocol.
		   @param connectionProtocol The protocol to use to connect to the %Photon servers. Must match one of the constants specified in ConnectionProtocol. 
		   @returns a reference to the instance on which it was called to allow for chaining multiple setter calls
		   @sa
		   getConnectionProtocol(), ConnectionProtocol */
		ClientConstructOptions& ClientConstructOptions::setConnectionProtocol(nByte connectionProtocol)
		{
			mConnectionProtocol = connectionProtocol;
			return *this;
		}
		
		/**
		   @returns the value of the autoLobbyStats flag
		   @sa
		   setAutoLobbyStats() */
		bool  ClientConstructOptions::getAutoLobbyStats(void) const
		{
			return mAutoLobbyStats;
		}
		
		/**
		   Sets the value of the autoLobbyStats flag.
		   @param autoLobbyStats Pass true, if you want to automatically receive updates for the lobby stats, false otherwise. Call opLobbyStats() to explicitly request a lobby stats update.
		   @returns a reference to the instance on which it was called to allow for chaining multiple setter calls
		   @sa
		   getAutoLobbyStats() */
		ClientConstructOptions& ClientConstructOptions::setAutoLobbyStats(bool autoLobbyStats)
		{
			mAutoLobbyStats = autoLobbyStats;
			return *this;
		}
		
		/**
		   @returns the currently set RegionSelectionMode
		   @sa
		   setRegionSelectionMode(), RegionSelectionMode */
		nByte ClientConstructOptions::getRegionSelectionMode(void) const
		{
			return mRegionSelectionMode;
		}
		
		/**
		   Sets the RegionSelectionMode.
		   @param regionSelectionMode Determines how the %Photon Cloud Region to which the Client connects should be selected. Must match one of the constants specified in RegionSelectionMode.
		   @returns a reference to the instance on which it was called to allow for chaining multiple setter calls
		   @sa
		   getRegionSelectionMode(), RegionSelectionMode */
		ClientConstructOptions& ClientConstructOptions::setRegionSelectionMode(nByte regionSelectionMode)
		{
			mRegionSelectionMode = regionSelectionMode;
			return *this;
		}
		
		/**
		   @returns the value of the useAlternativePorts flag
		   @sa
		   setUseAlternativePorts() */
		bool ClientConstructOptions::getUseAlternativePorts(void) const
		{
			return mUseAlternativePorts;
		}
		
		/**
		   Sets the value of the useAlternativePorts flag.
		   @param useAlternativePorts Determines if the the standard or the alternative port range should be used. This parameter currently is only relevant when ConnectionProtocol::UDP is passed
		   for parameter connectionProtocol and gets ignored otherwise. A router or firewall might block connections that use one port-range but don't block connections that use the other, so when
		   connecting with one range fails, then you may want to try with the other one.
		   @sa
		   getUseAlternativePorts() */
		ClientConstructOptions& ClientConstructOptions::setUseAlternativePorts(bool useAlternativePorts)
		{
			mUseAlternativePorts = useAlternativePorts; 
			return *this;
		}
		
		/**
		   @returns the currently set SerializationProtocol
		   @sa
		   setSerializationProtocol(), SerializationProtocol */
		nByte ClientConstructOptions::getSerializationProtocol(void) const
		{
			return mSerializationProtocol;
		}

		/**
		   Sets the SerializationProtocol.
		   @param serializationProtocol The protocol to use for serialization an deserialization of data. Must match one of the constants specified in SerializationProtocol.
		   @returns a reference to the instance on which it was called to allow for chaining multiple setter calls
		   @sa
		   getSerializationProtocol(), SerializationProtocol */
		ClientConstructOptions& ClientConstructOptions::setSerializationProtocol(nByte serializationProtocol)
		{
			mSerializationProtocol = serializationProtocol;
			return *this;
		}

		JString& ClientConstructOptions::toString(JString& retStr, bool /*withTypes*/) const
		{
			return retStr += JString(L"{connectionProtocol=") + getConnectionProtocol() + L", autoLobbyStats=" + getAutoLobbyStats() + L", regionSelectionMode=" + getRegionSelectionMode() + L", useAlternativePorts=" + getUseAlternativePorts() + L", serializationProtocol=" + getSerializationProtocol() + L"}";
		}
	}
}