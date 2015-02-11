/*************************************************************************/
/* spdlog - an extremely fast and easy to use c++11 logging library.     */
/* Copyright (c) 2015 Andrew Gunnerson <andrewgunnerson@gmail.com>       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#pragma once

#if defined(__ANDROID__) && defined(USE_ANDROID_LOG)

#include <array>
#include <string>
#include <android/log.h>

#include "./sink.h"
#include "../common.h"
#include "../details/log_msg.h"


namespace spdlog
{
namespace sinks
{
class androidlog_sink : public sink
{
public:
    //
    androidlog_sink(const std::string &tag = ""):
        _tag(tag)
    {
        _priorities[static_cast<int>(level::trace)] = ANDROID_LOG_DEBUG;
        _priorities[static_cast<int>(level::debug)] = ANDROID_LOG_DEBUG;
        _priorities[static_cast<int>(level::info)] = ANDROID_LOG_INFO;
        _priorities[static_cast<int>(level::notice)] = ANDROID_LOG_INFO;
        _priorities[static_cast<int>(level::warn)] = ANDROID_LOG_WARN;
        _priorities[static_cast<int>(level::err)] = ANDROID_LOG_ERROR;
        _priorities[static_cast<int>(level::critical)] = ANDROID_LOG_FATAL;
        _priorities[static_cast<int>(level::alert)] = ANDROID_LOG_ERROR;
        _priorities[static_cast<int>(level::emerg)] = ANDROID_LOG_FATAL;
        _priorities[static_cast<int>(level::off)] = ANDROID_LOG_VERBOSE;
    }

    androidlog_sink(const androidlog_sink&) = delete;
    androidlog_sink& operator=(const androidlog_sink&) = delete;

    void log(const details::log_msg &msg) override
    {
        ::__android_log_print(prio_from_level(msg), _tag.c_str(), "%s",
                              msg.formatted.str().c_str());
    }



private:
    std::array<int, 10> _priorities;
    const std::string _tag;

    int prio_from_level(const details::log_msg &msg) const
    {
        return _priorities[static_cast<int>(msg.level)];
    }
};
}
}

#endif
