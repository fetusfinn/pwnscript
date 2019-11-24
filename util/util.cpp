/*
 *  util.cpp
 */
#include <sys/stat.h>
#include <sstream>

#include "common.h"
#include "util.h"

namespace util
{
    /*
     *
     *
     */
    void exit(int code, std::string reason)
    {
        if(!reason.empty())
            pwned(reason);
        
        // close pointers
        // close modules
        
        /*
        if(code == EXIT_SUCCESS)
            pwned("EXIT_SUCCESS");
        else
            pwned("EXIT_FAILURE");
        */
        
        std::exit(code);
    }
    
    token_t get_token(token_type_t type, token_id_t id)
    {
        token_t token = "";
        
        try
        {
            token = g_token_list.at(type).at(id);
        }
        catch(std::exception& e)
        {
            pwned_error("Error getting token : " + std::to_string(type) + " | " + std::to_string(id));
        }
        
        return token;
    }
    
    /*
     *
     *
     */
    bool does_file_exist(const std::string& path)
    {
        std::ifstream f;
        f.open(path);
        bool good = f.good();
        f.close();
        return good;
    }
    
    /*
     *
     *
     */
    bool make_file(const std::string& path)
    {
        if(does_file_exist(path))
            return true;
        
        if(path.at(int(path.size() - 1)) == '/')
        {
            mkdir(path.c_str(), 0755);
        }
        else
        {
            std::ofstream f;
            f.open(path);
            f.close();
        }
        
        return does_file_exist(path);
    }
    
    /*
     *
     *
     */
    bool delete_file(const std::string& path)
    {
        unlink(path.c_str());
        return !does_file_exist(path);
    }
    
    /*
     *  dirname
     *  Returns the folder of the given path
     */
    std::string dirname(std::string path)
    {
        path.erase(std::find(path.rbegin(), path.rend(), '/').base(), path.end());
        return path;
    }
    
    /*
     *  explode
     *  Splits a strings at a given character then returns it as a vector, like php
     */
    std::vector<std::string> explode(const std::string& str, char delim, bool include)
    {
        std::vector<std::string> result;
        std::istringstream iss(str);
        
        for(std::string token; std::getline(iss, token, delim);)
        {
            result.push_back(std::move(token));
            
            if(include)
            {
                char a[1] = { delim };
                result.push_back(std::string(a));
            }
        }
        
        if(include)
            result.pop_back();
        
        return result;
    }
    
    /*
     *  explode
     *  Splits a strings at a given string then returns it as a vector, like php
     */
    std::vector<std::string> explode(std::string str, std::string delim, bool include)
    {
        // delim not in string, return the string
        if(str.find(delim) == std::string::npos)
            return { str };
        
        std::vector<std::string> exploded;
        
        size_t pos = 0;
        std::string token;
        
        while((pos = str.find(delim)) != std::string::npos)
        {
            token = str.substr(0, pos);
            
            // push back the token
            exploded.push_back(token);
            
            // include the delim with the vector?
            if(include)
            {
                // remove the token from the
                // start of the string
                str.erase(0, pos);
                
                // if the string is no more
                // then skip this part
                if(!str.empty())
                {
                    // we want to include the delim to push it back
                    exploded.push_back(delim);
                    
                    // remove the delim from the string
                    str.erase(0, delim.length());
                }
            }
            else
            {
                // we dont want to include so erase the token + the delim
                str.erase(0, pos + delim.length());
            }
        }
        
        return exploded;
    }
}
