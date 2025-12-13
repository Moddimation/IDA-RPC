#include "discord/rich_presence_helper.hpp"

#include <bytes.hpp>
#include <cstdint>
#include <idp.hpp>
#include <kernwin.hpp>
#include <loader.hpp>
#include <name.hpp>
#include <pro.h>

#include "discord/rich_presence.hpp"
#include "ida_helper.hpp"

extern idarpc::discord::RichPresence* rpc;

static ::discord::Timestamp start_time = (::discord::Timestamp)time( nullptr );

std::string get_ida_version_string()
{
        char version[ 32 ] = { 0 };
        get_kernel_version( version, sizeof( version ) );
        return std::string( version );
}
namespace idarpc::discord_rpc_helper
{
void update_presence( const discord_helper_spec& spec )
{
        if ( rpc == nullptr )
                return;

        ::discord::Activity activity{};
        activity.SetType( ::discord::ActivityType::Watching );
        activity.GetTimestamps().SetStart( start_time );

        std::string verstr  = idarpc::idahelper::is_ida_home_version() ? "IDA HOME " : "IDA PRO ";
        std::string version = verstr + get_ida_version_string();
        activity.GetAssets().SetLargeText( version.c_str() );
        activity.GetAssets().SetSmallText( "test" );

        uint8_t sha[ 256 ]{};
        retrieve_input_file_sha256( sha );

        activity.SetState( version.c_str() );
        std::string filename = "Analyzing ";
        filename += idahelper::get_filename();
        activity.SetDetails( filename.c_str() );

        rpc->update_presence( activity );
}

} // namespace idarpc::discord_rpc_helper
