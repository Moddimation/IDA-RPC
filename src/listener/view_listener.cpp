#include "view_listener.hpp"

#include <chrono>
#include <dbg.hpp>
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

bool try_get_name( std::string* str )
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

                ea_t ref = get_first_fcref_from( ea );
                if ( ref == BADADDR )
                        ref = get_first_dref_from( ea );

                if ( ref != BADADDR )
                {
                        qstring rname;

                        func_t* rfn = get_func( ref );
                        if ( rfn != nullptr )
                                get_func_name( &rname, rfn->start_ea );
                        else
                                rname = get_name( ref );

                        if ( !rname.empty() )
                        {
                                qsnprintf( buf, sizeof( buf ), "Ref: %s", rname.c_str() );
                                *str = buf;
                                return true;
                        }
                }

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

bool try_get_debugstate( std::string* str )
{
        if ( str == nullptr )
        {
                return false;
        }

        if ( !is_debugger_on() )
                return false;

        int state = get_process_state();

        if ( state == DSTATE_RUN )
        {
                *str = "Debugging: Running";
                return true;
        }

        if ( state == DSTATE_SUSP )
        {
                *str = "Debugging: Paused";
                return true;
        }

        if ( state == DSTATE_NOTASK )
        {
                *str = "Debugging: Not Connected";
                return true;
        }

        return true;
}

bool try_get_view( std::string* str, TWidget* view )
{
        qstring title;
        if ( !get_widget_title( &title, view ) )
        {
                return false;
        }

        *str = std::string( "View: " ) + title.c_str();
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
        std::string text = "Plugin Error";

        if ( !try_get_debugstate( &text ) && !try_get_name( &text ) && !try_get_view( &text, view ) )
        {
                text = "Decompiling";
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

