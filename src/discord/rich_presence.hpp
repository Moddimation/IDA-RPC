#pragma once

#include <discord.h>
#include <string>

namespace idarpc::discord
{

class RichPresence
{
public:
        explicit RichPresence( long long app_id );
        ~RichPresence();

        void update_presence( ::discord::Activity rpc );

        void clear_presence();
        void callbacks();

        bool is_alive();

private:
        long long        app_id_ = 0;
        ::discord::Core* discord = nullptr;
};

} // namespace idarpc::discord
