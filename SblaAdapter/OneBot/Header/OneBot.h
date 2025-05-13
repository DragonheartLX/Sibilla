#pragma once
#include <SblaInterface/IAdapter.h>
#include <curl/curl.h>

#include <string>

struct CurlWSStruct
{
	CURL* curl = nullptr;
	std::mutex msgMtx;
	std::string msg;
	std::function<void(void)> callBack;
};

size_t wsCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

class OneBot: public sbla::IAdapter
{
public:
	OneBot();
	~OneBot();

	virtual void receive() override;
	virtual void send() override;
	virtual void onExit() override;

	void msgCallBack();

private:
	std::string m_Url;
	std::string m_Token;

	CURLM* m_Curlm = nullptr;
	CURL* m_Curl   = nullptr;

	CurlWSStruct m_WSData;
};
