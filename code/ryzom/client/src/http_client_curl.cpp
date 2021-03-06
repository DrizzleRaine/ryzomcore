// Ryzom - MMORPG Framework <http://dev.ryzom.com/projects/ryzom/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdpch.h"
#include "http_client_curl.h"

#include <curl/curl.h>

#include "nel/gui/curl_certificates.h"

using namespace NLMISC;
using namespace NLNET;
using namespace std;

#ifdef DEBUG_NEW
#define new DEBUG_NEW
#endif

#define _Curl (CURL *)_CurlStruct


// Ugly CURL callback
size_t CCurlHttpClient::writeDataFromCurl(void *buffer, size_t size, size_t nmemb, void *pHttpClient)
{
	CCurlHttpClient * httpClient = static_cast<CCurlHttpClient*>(pHttpClient);
	httpClient->pushReceivedData((uint8*)buffer, (uint)(size*nmemb));
	return size*nmemb;
}

// ***************************************************************************
bool CCurlHttpClient::connect(const std::string &/* server */)
{
	curl_version_info_data *vdata = curl_version_info(CURLVERSION_NOW);
	nldebug("Libcurl v%s", vdata->version);
	if ((vdata->features & CURL_VERSION_SSL) == 0)
		nlwarning("SSL not supported");

	curl_global_init(CURL_GLOBAL_ALL);
	_CurlStruct = curl_easy_init();
	if(_Curl == NULL)
		return false;

	return true;
}

// ***************************************************************************
bool CCurlHttpClient::authenticate(const std::string &user, const std::string &password)
{
	_Auth = user + ":" + password;
	return true;
}

static const std::string CAFilename = "ssl_ca_cert.pem"; // this is the certificate "Thawte Server CA"

// ***************************************************************************
bool CCurlHttpClient::verifyServer(bool verify)
{
	curl_easy_setopt(_Curl, CURLOPT_SSL_VERIFYHOST, verify ? 2 : 0);
	curl_easy_setopt(_Curl, CURLOPT_SSL_VERIFYPEER, verify ? 1 : 0);

	// specify custom CA certs
	NLGUI::CCurlCertificates::addCertificateFile(CAFilename);

	// if supported, use custom SSL context function to load certificates
	NLGUI::CCurlCertificates::useCertificates(_Curl);

	return true;
}

// ***************************************************************************
bool CCurlHttpClient::sendRequest(const std::string& methodWB, const std::string &url, const std::string &cookieName, const std::string &cookieValue, const std::string& postParams, bool verbose)
{
	if (verbose)
		curl_easy_setopt(_Curl, CURLOPT_VERBOSE, 1);

	// Set URL
	curl_easy_setopt(_Curl, CURLOPT_URL, url.c_str());

	// Authentication
	if (!_Auth.empty())
	{
		curl_easy_setopt(_Curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC); // TODO: CURLAUTH_ANY
		curl_easy_setopt(_Curl, CURLOPT_USERPWD, _Auth.c_str());
	}

	// Set POST params
	if ((methodWB == "POST") && (!postParams.empty()))
	{
		curl_easy_setopt(_Curl, CURLOPT_POSTFIELDS, postParams.c_str());
	}
	// Cookie
	if (!cookieName.empty())
		curl_easy_setopt(_Curl,	CURLOPT_COOKIE, string(cookieName+"="+cookieValue).c_str());

	// Include the header in the response
	curl_easy_setopt(_Curl, CURLOPT_HEADER, 1);

	// Register the receive callback
	curl_easy_setopt(_Curl, CURLOPT_WRITEFUNCTION, CCurlHttpClient::writeDataFromCurl);
	curl_easy_setopt(_Curl, CURLOPT_WRITEDATA, this);

	char errorbuf [CURL_ERROR_SIZE+1];
	curl_easy_setopt(_Curl, CURLOPT_ERRORBUFFER, errorbuf);

	// Send
	CURLcode res = curl_easy_perform(_Curl);
	if (res != 0)
	{
		if (verbose)
			nlwarning(errorbuf);
		return false;
	}

	// Get result
	long r;
	curl_easy_getinfo(_Curl, CURLINFO_RESPONSE_CODE, &r);
	if (verbose)
	{
		nldebug("%u", (uint)r);
	}

	return true;
}

void CCurlHttpClient::pushReceivedData(uint8 *buffer, uint size)
{
	_ReceiveBuffer.insert(_ReceiveBuffer.end(), buffer, buffer+size);
}

// ***************************************************************************
bool CCurlHttpClient::sendGet(const string &url, const string& params, bool verbose)
{
	return sendRequest("GET", url + (params.empty() ? "" : ("?" + params)), string(), string(), string(), verbose);
}

// ***************************************************************************
bool CCurlHttpClient::sendGetWithCookie(const string &url, const string &name, const string &value, const string& params, bool verbose)
{
	return sendRequest("GET", url + (params.empty() ? "" : ("?" + params)), name, value, string(), verbose);
}

// ***************************************************************************
bool CCurlHttpClient::sendPost(const string &url, const string& params, bool verbose)
{
	return sendRequest("POST", url, string(), string(), params, verbose);
}

// ***************************************************************************
bool CCurlHttpClient::sendPostWithCookie(const string &url, const string &name, const string &value, const string& params, bool verbose)
{
	return sendRequest("POST", url, name, value, params, verbose);
}

// ***************************************************************************
bool CCurlHttpClient::receive(string &res, bool verbose)
{
	if (verbose)
	{
		nldebug("Receiving %u bytes", (uint)_ReceiveBuffer.size());
	}

	res.clear();
	if (!_ReceiveBuffer.empty())
		res.assign((const char*)&(*(_ReceiveBuffer.begin())), _ReceiveBuffer.size());
	_ReceiveBuffer.clear();
	return true;
}

// ***************************************************************************
void CCurlHttpClient::disconnect()
{
	curl_easy_cleanup(_Curl);
	curl_global_cleanup();
}

CCurlHttpClient CurlHttpClient;



