/*
 *  lexer.cpp
 */
#include "common.h"
#include "lexer.h"
#include "util.h"

// aka tokenizer
namespace lexer
{
    /*
     *  Has the lexer been initialised yet?
     */
    static bool m_init = false;
    
    /*
     *  Whether or not a multi line comment is open
     */
    static bool m_multi_line_open = false;
    
    /*
     *  List of tokens that seperate other tokens
     */
    std::vector<std::string> m_seperator_tokens;
    
    /*
     *  load_script
     *  Load the script into g_script line by line
     */
    script_t load_script(const std::string& script_path)
    {
        script_t script;
        
        std::ifstream file(script_path);
        
        line_t line;
        
        while(std::getline(file, line))
        {
            if(line.empty())
                continue;
            
            // save each line, comments will be filtered out later
            script.push_back(line);
        }
        
        file.close();
        
        pwned("Loaded script : " + script_path);
        
        return script;
    }
    
    /*
     *  init
     *  Initilaise the lexer
     */
    void init()
    {
        if(m_init)
            return;
        
        // Initiliase the seperator list
        
        // Load all symbols into the seperator list
        // because symbols are used to seerate text
        
        for(auto token : g_token_list[kSeperator])
            m_seperator_tokens.push_back(token);
        
        // Space is also a seperator but its not a token, " " is always the last seperator token
        m_seperator_tokens.push_back(" ");
        
        // dont need this but just to be safe
        m_init = true;
    }
    
    /*
     *  line
     *  Lex the line
     *  todo : line type too
     *  bug  : wont seperate at a full stop char, todo : if find(".") split at
     */
    lexed_line_t line(const line_t& line)
    {
        // pwned("Lexing line : " + line);
        
        // get the comment tokent
        token_t comment = util::get_token(kComment, kTokenCommentSingle);
        
        // see if theres a comment on this line
        size_t pos = line.find(comment);
        
        // the lexed line
        lexed_line_t lexed;
        
        // if the comment is in the line
        if(pos != std::string::npos)
        {
            // if pos is the last char
            if(line.length() == pos + 1)
            {
                lexed = util::explode(line, comment);
            }
            else
            {
                // not the last char so we are safe to do this
                char next_char = line.at(pos + 1);
                
                // todo : try filter out multi comment sections on the same line
                // e.g. : var v1 || multi line comment on single line || = 10
                // this should turn into : var v1 = 10
                
                // multi comments are ALWAYS comment + comment, e.g. comment = "/", multi_comment = "//"
                // the multi comment token opens and closes the comment
                
                // if the next char isnt another comment then we have a single comment
                // so explode it at the single cause we dont need the rest of the line
                // unless a multi comment is open, then we need to see if theres another
                // multi comment token so we have to keep the whole line
                if(next_char != comment.at(0) && !m_multi_line_open)
                    lexed = util::explode(line, comment);
                else
                    lexed = { line };
            }
        }
        else
        {
            lexed = { line };
        }
        
        lexed = util::explode(lexed.at(0), ' ');
        
        for(token_t sep : m_seperator_tokens)
        {
            // this seperator isnt found on this line, skip
            if(line.find(sep) == std::string::npos)
                continue;
            
            // try to split each element at the seperators
            // loop thru all elems in lexed, then try to split each one
            // at the current seperator
            for(int i = 0; i < lexed.size(); i++)
            {
                // an unseperated part of the line
                // "var", "v1," "v2", "=", "3.14,", "10"
                token_t unlexed = lexed.at(i);
                
                // notice the commas are now seperate
                // var, "v1", ",", "v2", "=", "3.14", ",", "10"
                lexed_line_t lex = util::explode(unlexed, sep, true);
                
                if(lex.size() > 1)
                {
                    lexed.erase(lexed.begin() + i);
                    lexed.insert(lexed.begin() + i, lex.begin(), lex.end());
                    
                    i += lex.size();
                }
            }
        }
        
        // bug : double quotes " add a empty character before them in the vector
        
        // filter out all the empty elements
        if(std::find(lexed.begin(), lexed.end(), "") != lexed.end())
            lexed.erase(std::remove(lexed.begin(), lexed.end(), ""));
        
        
        // make it look good
        size_t s = lexed.size();
        if(s)
        {
            std::string p = "   { ";
            for(int i = 0; i < s; i++)
                p += "\"" + lexed.at(i) + "\"" + (i == s - 1 ? "" : ", ");
            p += " }";
            pwned(p);
        }
        
        return lexed;
    }
    
    /*
     *  update_multi_line_open
     *
     */
    void update_multi_line_open(lexed_line_t tokens)
    {
        const token_t comment = util::get_token(kComment, kTokenCommentSingle);
        
        for(auto token : tokens)
        {
            if(token == comment + comment)
                m_multi_line_open = !m_multi_line_open;
        }
    }
    
    /*
     *  start
     *  Start lexing the script
     */
    // lexed_script_t script(const script_t& script, bool reload)
    lexed_script_t script(const std::string& script_path, bool reload)
    {
        pwned("Lexing script");
        
        // get the full path of the script
        char _real[PATH_MAX];
        realpath(script_path.c_str(), _real);
        const std::string real = std::string(_real);
        
        // already loaded so return the loaded script && we dont want to reload it
        if(g_lexed_scripts.find(real) != g_lexed_scripts.end() && !reload)
            return g_lexed_scripts.at(real);
        
        // new script so reset multi line flag
        m_multi_line_open = false;
        
        // load script line by line
        script_t script = lexer::load_script(script_path);
        
        size_t script_size = script.size();
        
        pwned("{ ");
        
        lexed_script_t lexed_script;
        
        for(int line_num = 0; line_num < script_size; line_num++)
        {
            line_t line = script.at(line_num);
            
            lexed_line_t lexed = lexer::line(line);
            
            if(lexed.empty())
                continue;
            
            lexer::update_multi_line_open(lexed);
            
            lexed_script.push_back(lexed);
        }
        
        pwned("}");
        pwned("Lexing finished");
        
        // add the lexed script to the map
        g_lexed_scripts.insert(std::make_pair(real, lexed_script));
        
        // and return it
        return lexed_script;
    }
}

