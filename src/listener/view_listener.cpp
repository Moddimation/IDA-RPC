#include "view_listener.hpp"

#include <chrono>
#include <fstream>
#include <funcs.hpp>
#include <hexrays.hpp>
#include <kernwin.hpp>
#include <mutex>
#include <pro.h>

#include "discord/rich_presence_helper.hpp"
#include "log/log.hpp"

std::string cache_string = "";

namespace idarpc::listener
{

bool try_get_func_name( std::string* str )
{
        if ( str == nullptr )
        {
                return false;
        }

        ea_t ea = BADADDR;
        callui( ui_screenea, &ea );
        if ( ea == BADADDR )
        {
                return false;
        }

        func_t* fn = get_func( ea );
        if ( fn == nullptr )
        {
                char buf[ 1024 ];

                qstring nm = get_name( ea );
                if ( !nm.empty() )
                {
                        qsnprintf( buf, sizeof( buf ), "At: %s", nm.c_str() );
                        *str = buf;
                        return true;
                }

                qsnprintf( buf, sizeof( buf ), "Addr: 0x%llX", (unsigned long long)ea );
                *str = buf;
                return true;
        }

        qstring name;
        get_func_name( &name, fn->start_ea );

        *str = std::string( "Func: " ) + name.c_str();
        return true;
}

void update_view_presence( const std::string text )
{
        if ( text == cache_string )
                return;
        cache_string = text;
        discord_helper_spec spec;
        spec.details = text;
        discord_rpc_helper::update_presence( spec );
}

void do_update_view_presence( TWidget* view )
{
        std::string text = "Decompiling";
        if ( !try_get_func_name( &text ) )
        {
                qstring title;
                if ( get_widget_title( &title, view ) )
                        text = std::string( "View: " ) + title.c_str();
        }
        update_view_presence( text );
}

ssize_t idaapi ViewListener::on_event( ssize_t code, va_list va )
{
        TWidget* view = va_arg( va, TWidget* );

        do_update_view_presence( view );

        return 0;
}
} // namespace idarpc::listener

