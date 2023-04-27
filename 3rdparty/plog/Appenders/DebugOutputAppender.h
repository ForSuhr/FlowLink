#pragma once
#include <Appenders/IAppender.h>
#include <WinApi.h>

namespace plog
{
    template<class Formatter>
    class PLOG_LINKAGE_HIDDEN DebugOutputAppender : public IAppender
    {
    public:
        virtual void write(const Record& record) PLOG_OVERRIDE
        {
            OutputDebugStringW(Formatter::format(record).c_str());
        }
    };
}
