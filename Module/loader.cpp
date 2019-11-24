/*
 *  loader.cpp
 */
#include "common.h"
#include "module.h"
#include "loader.h"

namespace loader
{
    std::vector<modules::c_module> mod_list;
    
    void find_modules()
    {
        // todo
        // dirent all dylibs
    }
    
    void load_module(const std::string& path)
    {
        // todo
        // move the given module into g_pwn_script_folder + "modules/user/" or something
    }
}
