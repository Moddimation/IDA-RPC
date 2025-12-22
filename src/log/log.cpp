#include "log.hpp"

#include <chrono>
#include <ctime>
#include <idp.hpp>
#include <iomanip>
#include <sstream>

#define WRITEDEBUGFILE 0
#ifdef WRITEDEBUGFILE
#include <cstdio>
#include <mutex>
#endif

namespace
{
#ifdef WRITEDEBUGFILE
FILE* get_log_file()
{
        static FILE* file = nullptr;
        if ( !file )
                file = qfopen( "ida_rpc.log", "a" );
        return file;
}

std::mutex log_mutex;
#endif
} // namespace

void idarpc::log( LogLevel level, const std::string& content )
{
        auto        now   = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t( now );

        std::stringstream time_ss;
        time_ss << std::put_time( std::localtime( &now_c ), "%Y-%m-%d %H:%M:%S" );

        const char* level_name = get_log_level_name( level );

        msg( "[IDARPC] [%s] [%s] %s\n", time_ss.str().c_str(), level_name, content.c_str() );

#ifdef WRITEDEBUGFILE
        {
                std::lock_guard<std::mutex> lock( log_mutex );
                FILE*                       file = get_log_file();
                if ( file )
                {
                        qfprintf( file, "[IDARPC] [%s] [%s] %s\n", time_ss.str().c_str(), level_name, content.c_str() );
                        qflush( file );
                }
        }
#endif
}

