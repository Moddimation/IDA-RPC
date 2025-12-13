#pragma once

#include <string>

namespace idarpc::idahelper
{
std::string get_filename();
std::string get_projectname();
std::string get_ida_version_string();
bool        is_ida_home_version();
} // namespace idarpc::idahelper
