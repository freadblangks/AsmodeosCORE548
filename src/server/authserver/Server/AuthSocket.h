/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SF_AUTHSOCKET_H
#define SF_AUTHSOCKET_H

#include "Common.h"
#include "BigNumber.h"
#include "RealmSocket.h"

class ACE_INET_Addr;
struct Realm;

// Handle login commands
class AuthSocket: public RealmSocket::Session
{
public:
    const static int s_BYTE_SIZE = 32;

    AuthSocket(RealmSocket& socket);
    virtual ~AuthSocket(void);

    virtual void OnRead(void);
    virtual void OnAccept(void);
    virtual void OnClose(void);

    static ACE_INET_Addr const& GetAddressForClient(Realm const& realm, ACE_INET_Addr const& clientAddr);

    bool _HandleLogonChallenge();
    bool _HandleLogonProof();
    bool _HandleReconnectChallenge();
    bool _HandleReconnectProof();
    bool _HandleRealmList();

    //data transfer handle for patch
    bool _HandleXferResume();
    bool _HandleXferCancel();
    bool _HandleXferAccept();

    void _SetVSFields(const std::string& rI);

    FILE* pPatch;
    ACE_Thread_Mutex patcherLock;

private:
    RealmSocket& socket_;
    RealmSocket& socket(void) { return socket_; }

    BigNumber N, s, g, v;
    BigNumber b, B;
    BigNumber K;
    BigNumber _reconnectProof;

    bool _authed;

    std::string _login;
    std::string _tokenKey;

    // Since GetLocaleByName() is _NOT_ bijective, we have to store the locale as a string. Otherwise we can't differ
    // between enUS and enGB, which is important for the patch system
    std::string _localizationName;
    std::string _os;
    uint16 _build;
    uint8 _expversion;
    AccountTypes _accountSecurityLevel;
};

#endif