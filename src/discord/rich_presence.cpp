#include "rich_presence.hpp"

#include <sstream>

#include "log/log.hpp"

typedef ::discord::Result Res;

namespace idarpc::discord
{
RichPresence::RichPresence( long long app_id ) : app_id_( app_id )
{
        idarpc::log( LogLevel::Info, "Initializing Rich Presence..." );
        Res res = ::discord::Core::Create( app_id, DiscordCreateFlags_NoRequireDiscord, &discord );
        idarpc::log( LogLevel::Info, "Checking for errors..." );
        if ( res != Res::Ok )
        {
                if ( res == Res::NotRunning || res == Res::ServiceUnavailable )
                {
                        idarpc::log( LogLevel::Warning, "Discord not started, RPC stop." );
                }
                else
                {
                        idarpc::log( LogLevel::Warning, "Could not connect to Discord, RPC stop." );
                }
                discord = nullptr;
                return;
        }
}

RichPresence::~RichPresence()
{
        clear_presence();
        if ( is_alive() )
        {
                delete discord;
                discord = nullptr;
                idarpc::log( LogLevel::Info, "Shutdown Rich Presence." );
        }
}

bool RichPresence::is_alive()
{
        return discord != nullptr;
}

void RichPresence::update_presence( ::discord::Activity rpc )
{
        if ( !is_alive() )
                return;

        discord->ActivityManager().UpdateActivity( rpc, []( Res result )
                {
                        if (result != Res::Ok) {
                            idarpc::log( LogLevel::Error, "Failed to update Presence." );
                        } } );
}

void RichPresence::callbacks()
{
        discord->RunCallbacks();
}

void RichPresence::clear_presence()
{
        if ( is_alive() )
        {
                discord->ActivityManager().ClearActivity( []( Res result )
                        {
                                if ( result != Res::Ok )
                                {
                                        idarpc::log( LogLevel::Error, "Failed to clear Presence." );
                                }
                                else
                                {
                                        idarpc::log( LogLevel::Info, "Presence cleared." );
                                } } );
        }
}
} // namespace idarpc::discord
