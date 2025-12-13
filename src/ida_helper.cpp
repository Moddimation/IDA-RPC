#include "ida_helper.hpp"

#include <bytes.hpp>
#include <diskio.hpp>
#include <idp.hpp>
#include <kernwin.hpp>
#include <loader.hpp>
#include <name.hpp>
#include <pro.h>

#include "log/log.hpp"

std::string idarpc::idahelper::get_filename()
{
        char filename_buf[ 260 ] = {};
        if ( !get_root_filename( filename_buf, sizeof( filename_buf ) ) )
                return std::string( "undefined" );

        return std::string( filename_buf );
}
std::string idarpc::idahelper::get_projectname()
{
        const char* full = get_path( PATH_TYPE_IDB );
        const char* base = qbasename( full );
        if ( base == nullptr )
                return std::string( "undefined" );

        return std::string( base );
}
std::string idarpc::idahelper::get_ida_version_string()
{
        char version[ 32 ] = { 0 };
        if ( !get_kernel_version( version, sizeof( version ) ) )
                return std::string( "undefined" );

        return std::string( version );
}
bool idarpc::idahelper::is_ida_home_version()
{
        qstrvec_t dirs;
        get_ida_subdirs( &dirs, nullptr );

        for ( const auto& dir : dirs )
        {
                std::string path = dir.c_str();

                path.erase( 0, path.find_first_not_of( " \t\r\n" ) );
                path.erase( path.find_last_not_of( " \t\r\n" ) + 1 );

                std::string clean_path;
                for ( char c : path )
                {
                        if ( c == '\\' )
                                c = '/';
                        if ( c != ' ' && c != '\t' )
                        {
                                clean_path += std::tolower( static_cast<unsigned char>( c ) );
                        }
                }

                if ( clean_path.find( "idahome" ) != std::string::npos )
                {
                        return true;
                }
        }

        return false;
}
