#pragma once

#include <cstdint>
#include <string>

#include "SblaCore/DataProtocol/Protocol.h"

namespace sbla
{
	struct Response
	{
		std::string status	= "";
		int64_t retcode		= 0;

		SblaMap data		= {};
		std::string message = "";

		std::string echo	= "";
	};

	/*
        retcode

            0 OK
        1xxxx Request Error
            10001 Bad Request
            10002 Unsupported Action
            10003 Bad Param
            10004 Unsupported Param
            10005 Unsupported Segment
            10006 Bad Segment Data
            10007 Unsupported Segment Data
            10101 Who Am I
            10102 Unknown Self
        
        2xxxx Handler Error
            20001 Bad Handler
            20002 Internal Handler Error

        3xxxx Execution Error
            31xxx Database Error
            32xxx Filesystem Error
            33xxx Network Error
            34xxx Platform Error
            35xxx Logic Error
            36xxx I Am Tired

        4xxxx、5xxxx

        6xxxx～9xxxx
    */
} // namespace sbla