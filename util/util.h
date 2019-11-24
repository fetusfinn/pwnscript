/*
 *  util.h
 */
#pragma once

namespace util
{
    /*
     *  Important / useful
     */
    
    void    exit(int code, std::string reason = "");
    token_t get_token(token_type_t type, token_id_t id);
    
    /*
     *  File system
     */
    
    bool does_file_exist(const std::string& path);
    bool make_file(const std::string& path);
    bool delete_file(const std::string& path);
    std::string dirname(std::string path);
    
    
    /*
     *  String
     */
    
    
    std::vector<std::string> explode(const std::string& str, char delim, bool include = false);
    std::vector<std::string> explode(std::string to_explode, std::string explode_at, bool include = false);
    
    
    
    /*
     *  Logging / output
     */
    
    template<class type>
    void message(type msg, std::string prefix = "")
    {        
        if(!prefix.empty())
            prefix  = "[" + prefix + "] ";
        
        // no need for std::endl
        std::cout << prefix << msg << "\n";
    }
}

#define pwned(msg) util::message(msg, "pwned")
#define pwned_error(msg) util::message(msg, "error")
