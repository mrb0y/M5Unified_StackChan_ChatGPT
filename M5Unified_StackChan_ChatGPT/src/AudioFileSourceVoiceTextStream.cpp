#if defined(ESP32) || defined(ESP8266)
#pragma once

#include <Arduino.h>
#include <HTTPClient.h>
#include <aws/core/Aws.h>
#include <aws/polly/PollyClient.h>
#include <aws/polly/model/SynthesizeSpeechRequest.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include "AudioFileSource.h"

extern Aws::Auth::AWSCredentials credentials; // AWS credentials: you need to set these up elsewhere in your code

class AudioFileSourceAmazonPollyStream : public AudioFileSource
{
  public:
    AudioFileSourceAmazonPollyStream();
    AudioFileSourceAmazonPollyStream(const char *tts_text, const char *tts_parms);
    virtual ~AudioFileSourceAmazonPollyStream() override;
    
    virtual bool open(const char *url) override;
    virtual uint32_t read(void *data, uint32_t len) override;
    virtual bool seek(int32_t pos, int dir) override;
    virtual bool close() override;
    virtual bool isOpen() override;
    virtual uint32_t getSize() override;
    virtual uint32_t getPos() override;
    bool SetReconnect(int tries, int delayms) { reconnectTries = tries; reconnectDelayMs = delayms; return true; }

    enum { STATUS_HTTPFAIL=2, STATUS_DISCONNECTED, STATUS_RECONNECTING, STATUS_RECONNECTED, STATUS_NODATA };

  private:
    virtual uint32_t readInternal(void *data, uint32_t len, bool nonBlock);
    WiFiClient client;
    HTTPClient http;
    int pos;
    int size;
    int reconnectTries;
    int reconnectDelayMs;
    char saveURL[128];
    const char *text;
    const char *parms;
    Aws::Polly::PollyClient pollyClient;
    Aws::Polly::Model::SynthesizeSpeechRequest synthesizeSpeechRequest;
};

#endif
