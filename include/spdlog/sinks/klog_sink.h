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

#ifdef __linux__

#include <array>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "./sink.h"
#include "../common.h"
#include "../details/log_msg.h"


namespace spdlog
{
namespace sinks
{
class klog_sink : public sink
{
public:
    //
    klog_sink(const std::string& tag = ""):
        _tag(tag)
    {
        // http://elinux.org/Debugging_by_printing#Log_Levels
        _priorities[static_cast<int>(level::trace)]    = "<7>"; // KERN_DEBUG
        _priorities[static_cast<int>(level::debug)]    = "<7>"; // KERN_DEBUG
        _priorities[static_cast<int>(level::info)]     = "<6>"; // KERN_INFO
        _priorities[static_cast<int>(level::notice)]   = "<5>"; // KERN_NOTICE
        _priorities[static_cast<int>(level::warn)]     = "<4>"; // KERN_WARNING
        _priorities[static_cast<int>(level::err)]      = "<3>"; // KERN_ERR
        _priorities[static_cast<int>(level::critical)] = "<2>"; // KERN_CRIT
        _priorities[static_cast<int>(level::alert)]    = "<1>"; // KERN_ALERT
        _priorities[static_cast<int>(level::emerg)]    = "<0>"; // KERN_EMERG
        _priorities[static_cast<int>(level::off)]      = "<d>"; // KERN_DEFAULT

        static int open_mode = O_WRONLY | O_NOCTTY | O_CLOEXEC;
        static const char *kmsg = "/dev/kmsg";
        static const char *kmsg2 = "/dev/kmsg.temp";

        _fd = ::open(kmsg, open_mode);
        if (_fd < 0) {
            // If /dev/kmsg hasn't been created yet, then create our own
            // Character device mode: S_IFCHR | 0600
            //
            if (::mknod(kmsg2, S_IFCHR | 0600, makedev(1, 11)) == 0) {
                _fd = ::open(kmsg2, open_mode);
                if (_fd >= 0) {
                    ::unlink(kmsg2);
                }
            }
        }
    }
    ~klog_sink()
    {
        if (_fd >= 0) {
            close(_fd);
        }
    }

    klog_sink(const klog_sink&) = delete;
    klog_sink& operator=(const klog_sink&) = delete;

    void log(const details::log_msg &msg) override
    {
        if (_fd >= 0) {
            std::string logmsg = klog_prio_from_level(msg);
            logmsg += msg.formatted.str();
            ::write(_fd, logmsg.data(), logmsg.size());
        }
    }



private:
    std::array<std::string, 10> _priorities;
    const std::string _tag;
    int _fd;

    std::string klog_prio_from_level(const details::log_msg &msg) const
    {
        return _priorities[static_cast<int>(msg.level)];
    }
};
}
}

#endif
