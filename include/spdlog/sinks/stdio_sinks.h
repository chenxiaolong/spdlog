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

#include <string>

#include "./sink.h"
#include "../common.h"
#include "../details/log_msg.h"


namespace spdlog
{
namespace sinks
{
class stdio_sink : public sink
{
public:
    //
    stdio_sink(FILE *stream, const std::string &tag = ""):
        _stream(stream), _tag(tag)
    {
    }

    stdio_sink(const stdio_sink&) = delete;
    stdio_sink& operator=(const stdio_sink&) = delete;

    void log(const details::log_msg &msg) override
    {
        fprintf(_stream, "%s", msg.formatted.str().c_str());
    }



private:
    std::FILE *_stream;
    const std::string _tag;
};

class c_stdout_sink : public stdio_sink
{
public:
    c_stdout_sink(const std::string &tag = "") : stdio_sink(stdout, tag)
    {
    }
};

class c_stderr_sink : public stdio_sink
{
public:
    c_stderr_sink(const std::string &tag = "") : stdio_sink(stderr, tag)
    {
    }
};
}
}
