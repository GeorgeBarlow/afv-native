/* afv/APISession.h
 *
 * This file is part of AFV-Native.
 *
 * Copyright (c) 2019 Christopher Collins
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AFV_NATIVE_APISESSION_H
#define AFV_NATIVE_APISESSION_H

#include "afv-native/afv/dto/Station.h"
#include "afv-native/afv/dto/StationTransceiver.h"
#include "afv-native/event.h"
#include "afv-native/event/EventCallbackTimer.h"
#include "afv-native/http/RESTRequest.h"
#include "afv-native/http/Request.h"
#include "afv-native/http/TransferManager.h"
#include "afv-native/util/ChainedCallback.h"
#include <event2/event.h>
#include <map>
#include <memory>
#include <string>

namespace afv_native { namespace afv {
    class VoiceSession;

    class APISession {
      public:
        APISession(event_base *evBase, http::TransferManager &tm, std::string baseUrl, std::string clientName);

        const std::string &getUsername() const;

        void setUsername(const std::string &username);

        void setPassword(const std::string &password);

        void setAuthenticationFor(http::Request &r);

        http::TransferManager &getTransferManager() const;

        struct event_base *getEventBase() const;

        APISessionState getState() const;

        const std::string &getBaseUrl() const;
        void               setBaseUrl(std::string newUrl);

        void Connect();
        void Disconnect();

        APISessionError getLastError() const;

        void                      updateStationAliases();
        void                      requestStationTransceivers(std::string stationName);
        void                      requestStationVccs(std::string stationName);
        void                      getStation(std::string stationName);
        std::vector<dto::Station> getStationAliases() const;
        std::map<std::string, std::vector<dto::StationTransceiver>> getStationTransceivers() const;

        /** Callbacks registered against StateCallback will be called whenever
         * the APISession changes state.
         */
        util::ChainedCallback<void(APISessionState)> StateCallback;
        util::ChainedCallback<void(void)>            AliasUpdateCallback;
        util::ChainedCallback<void(std::string)>     StationTransceiversUpdateCallback;
        util::ChainedCallback<void(std::string, std::map<std::string, unsigned int>)> StationVccsCallback;
        util::ChainedCallback<void(bool, std::pair<std::string, unsigned int>)> StationSearchCallback;

      protected:
        void _authenticationCallback(http::RESTRequest *req, bool success);
        void _stationsCallback(http::RESTRequest *req, bool success);
        void _stationTransceiversCallback(http::RESTRequest *req, bool success, std::string stationName);
        void _stationVccsCallback(http::RESTRequest *req, bool success, std::string stationName);
        void _getStationCallback(http::RESTRequest *req, bool success, std::string stationName);
        void setState(APISessionState newState);
        void raiseError(APISessionError error);

        struct event_base     *mEvBase;
        http::TransferManager &mTransferManager;
        std::string            mBaseURL;
        std::string            mUsername;
        std::string            mPassword;
        std::string            mClientName;

        std::string mBearerToken;

        http::RESTRequest mAuthenticationRequest;

        event::EventCallbackTimer mRefreshTokenTimer;

        APISessionError mLastError;

        http::RESTRequest                                           mStationAliasRequest;
        http::RESTRequest                                           mStationTransceiversRequest;
        http::RESTRequest                                           mGetStationRequest;
        http::RESTRequest                                           mVccsRequest;
        std::vector<dto::Station>                                   mAliasedStations;
        std::map<std::string, std::vector<dto::StationTransceiver>> mStationTransceivers;

      private:
        APISessionState mState;
    };
}} // namespace afv_native::afv

#endif // AFV_NATIVE_APISESSION_H