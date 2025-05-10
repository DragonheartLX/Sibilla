#include "Utils/Utils.h"

namespace scli
{
    size_t wsCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        CurlWSStruct* self = static_cast<CurlWSStruct*>(userdata);
        const struct curl_ws_frame* frame = curl_ws_meta(self->curl);
        size_t result = size * nmemb;

        {
            std::lock_guard<std::mutex> lock(self->msgMtx);
            self->msg.append(ptr, result);
        }

        if (frame->bytesleft > 0) return result;

        self->callBack();

        {
            std::lock_guard<std::mutex> lock(self->msgMtx);
            self->msg.clear();
        }
        return result;
    }
} // namespace scli