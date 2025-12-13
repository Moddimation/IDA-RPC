#include "discord/rich_presence_helper.hpp"

#include <cstdint>
#include <ctime>

#include "discord/rich_presence.hpp"
#include "ida_helper.hpp"

extern idarpc::discord::RichPresence* rpc;

static ::discord::Timestamp start_time = (::discord::Timestamp)time( nullptr );

namespace idarpc::discord_rpc_helper
{
void update_presence( const discord_helper_spec& spec )
{
        if ( rpc == nullptr )
                return;

        std::string stat;
        stat += "v" + idahelper::get_ida_version_string();
        stat += " / " + idahelper::get_filename();
        stat += " / " + idahelper::get_projectname();

        ::discord::Activity activity{};
        activity.GetTimestamps().SetStart( start_time );

        activity.SetState( stat.c_str() );
        activity.SetDetails( spec.details.c_str() );

        rpc->update_presence( activity );
}

} // namespace idarpc::discord_rpc_helper
