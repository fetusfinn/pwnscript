/*
 *  parser.cpp
 */
#include "common.h"
#include "parser.h"
#include "lexer.h"
#include "util.h"

namespace parser
{
    // todo
    // The Parser is the part that evaluates the tokens, such as their order and number of parameters
    
    void start(const lexed_script_t& script, const std::string& path)
    {
        pwned("Started parsing");
        pwned("");
        
        for(int line_num = 0; line_num < script.size(); line_num++)
        {
            lexed_line_t line = script.at(line_num);
            
            size_t token_count = line.size();
            
            // do while so we can use break to exit the line
            do
            {
                for(int token_num = 0; token_num < token_count; token_num++)
                {
                    token_t token = line.at(token_num);
                    
                    // first token
                    if(token_num == 0)
                    {
                        // todo : find better way to do this
                        
                        if(token == util::get_token(kKeyword, kTokenUse))
                        {
                            // next token should be the module path
                            // in the correct format
                            // e.g. "path.to.module"
                            // e.g. "sys.fs"

                            // the line is just "use" and nothing else
                            if(token_num + 1 >= token_count)
                                continue;
                            
                            std::string module_path = line.at(token_num + 1);
                            std::replace(module_path.begin(), module_path.end(), '.', '/');
                            
                            // todo : check for dylib modules too
                            module_path += ".pw";
                            
                            std::string temp = util::dirname(g_script_path) + module_path;
                            
                            // check for the module relative to the script
                            if(!util::does_file_exist(temp))
                            {
                                temp = g_pwn_script_folder + "sys/" + module_path;
                                
                                // check for pwnscript modules
                                if(!util::does_file_exist(temp))
                                {
                                    temp = g_pwn_script_folder + "usr/" + module_path;
                                    
                                    // check for user loaded modules
                                    if(!util::does_file_exist(temp))
                                    {
                                        pwned_error("Module not found : " + module_path);
                                        break;
                                    }
                                }
                            }
                            
                            module_path = temp;
                            
                            pwned("Module found : " + module_path);
                            
                            // lex and parse script
                            {
                                lexed_script_t lexed = lexer::script(module_path);
                                
                                // parser::start(lexed, module_path);
                            }
                            
                            // todo...
                            
                            // finished loading, skip everything after this
                            break;
                        }
                        else if(token == util::get_token(kKeyword, kTokenGlobal))
                        {
                            // store variables into g_variables
                        }
                        else if(token == util::get_token(kKeyword, kTokenVar))
                        {
                            // store variables into g_variables
                        }
                        // todo...
                    }
                }
            } while(0);
        }
        
        pwned("");
        pwned("Finished parsing");
    }
}
