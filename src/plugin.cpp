#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>

#include "plugin_mod.hpp"

static plugmod_t* idaapi init( void )
{
        ida_rpc_mod* plug = new ida_rpc_mod();
        if ( !plug->is_alive() )
        {
                delete plug;
                plug = nullptr;
        }

        return (plugmod_t*)plug;
}

plugin_t PLUGIN = {
        IDP_INTERFACE_VERSION,
        PLUGIN_HIDE,
        init,
        nullptr,
        nullptr,
        "Adds Discord Rich Presence integration for IDA.",
        "Discord Rich Presence",
        "IDA Discord RPC",
        "Created by Reversed Codes, updated by Moddimation",
};
