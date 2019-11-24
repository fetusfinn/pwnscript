/*
 *  pwnscript
 *
 *  slanguage
 *
 *  Notes :
 *
 *  ALL SCRIPT FILES MUST END WITH .PW
 *
 *  Check all "todo"'s
 *
 *  Lexer   | script loader
 *  Flex    |
 *  Parser  | match tokens together to form working code
 *
 *  Add dylib loader so c++ modules can be used
 *
 *  "Writing a language is hard while high"
 *
 *  Links :
 *
 *  https://stackoverflow.com/questions/40009776/writing-a-scripting-language
 *  https://www.freecodecamp.org/news/the-programming-language-pipeline-91d3f449c919/
 *  https://stackoverflow.com/questions/2684603/how-do-i-initialize-a-float-to-its-max-min-value | type sizes and ranges
 *
 */
#include "common.h"
#include "util/util.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "module/loader.h"

/*
 *  Global variables
 *  Move to another file
 */

// the path to the pwnscript folder where everything is
const std::string g_pwn_script_folder = std::string(std::getenv("HOME")) + "/.pwnscript/";

// the path to the script that we are running
std::string g_script_path;

// all lexed scripts and their path
std::unordered_map<std::string, lexed_script_t> g_lexed_scripts;

// a list of all pre defined tokens, loaded from the file : m_token_list_path
std::unordered_map<token_type_t, std::vector<token_t>> g_token_list;

// all registered variables
std::vector<variable_t> g_variables;

/*
 *  Local variables
 */

// the path to the token list
static std::string m_token_list_path = g_pwn_script_folder + "tokens.pw";

/*
 *  parse_arguments
 *  Parses the arguments given to the program
 */
void parse_arguments(int argc, const char* argv[])
{
    // script path is always the last argument
    // todo : ^ dont do that, find better way, but for now this is fine
    
    g_script_path = argv[argc - 1];
    
    pwned("Running script : " + g_script_path);
    
    // loop thru all args
    for(int i = 1; i < argc; i++)
    {
        const char* arg = argv[i];

        // load new dylib module
        if(!strcmp(arg, "load"))
        {
            if(argc <= i + 1)
                continue;
            
            std::string mod_path = std::string(argv[i + 1]);
            
            pwned("Loading module : " + mod_path);
            
            loader::load_module(mod_path);
        }
        // token list path
        else if(!strcmp(arg, "-l"))
        {
            if(argc <= i + 1)
                continue;
            
            m_token_list_path = argv[i + 1];
        }
        else if(!strcmp(arg, "-d") || !strcmp(arg, "debug"))
        {
            // sleep for a bit so we can wait for the debugger to attach
            // with xcode once it's attached itll skip past this sleep
            sleep(60);
        }
    }
}

/*
 *  load_token_list
 *  Loads the tokens from the token list file into the interpreter
 */
bool load_token_list()
{
    pwned("Loading token list : " + m_token_list_path);
    
    if(!util::does_file_exist(m_token_list_path))
        util::exit(1, "Token list not found!");
    
    //  token list file layout
    //
    //  # token_type_id
    //
    //  token_t | documentation
    //  token_t | documentation
    //
    
    // current line
    line_t line;

    // the current type of tokens that we are importing
    // kinda like a section
    token_type_t type = kInvalidType;
    
    // the tokens of the type ^ and all tokens that we have imported so far
    std::vector<token_t> current_tokens, all_tokens;
    
    // token list file
    std::ifstream token_list_file(m_token_list_path);
    
    // read it line by line
    while(std::getline(token_list_file, line))
    {
        // empty so just skip
        if(line.empty())
            continue;
        
        // explode the line at each space
        auto exploded = util::explode(line, ' ');
        
        // empty so skip
        if(exploded.empty())
            continue;
        
        // the token we want is always the first one, except for the section headers
        // everything after this will be documentation
        token_t token = exploded.at(0);
        
        // new section
        if(token == "#")
        {
            // add the last type of tokens to the list
            if(type != kInvalidType)
                g_token_list.insert(std::make_pair(type, current_tokens));
            
            // try get the section
            try
            {
                type = (token_type_t)std::stoi(exploded.at(exploded.size() - 1));
            }
            catch(std::exception& e)
            {
                type = kInvalidType;
            }            
            
            current_tokens.clear();
            
            continue;
        }
        
        // token has already been loaded, skip line
        if(type != kInvalidType && std::find(all_tokens.begin(), all_tokens.end(), token) != all_tokens.end())
        {
            // comments are all throughout the tokens file, we only want to get it from the comment section
            if(type != kComment)
                continue;
        }
        
        all_tokens.push_back(token);
        current_tokens.push_back(token);
    }
    
    // need to do this otherwise the last
    // section wont be stored
    if(type != kInvalidType)
        g_token_list.insert(std::make_pair(type, current_tokens));
    
    // open token_list
    // read token_list line by line
    // explode each line at " " and only take the first element, this should be the token, everything else is documentation
    // store the token into g_token_list
    token_list_file.close();
    
    int count = 0;
    
    for(auto v : g_token_list)
        count += v.second.size();
    
    pwned(std::to_string(count) + " tokens loaded");
    
    return !g_token_list.empty();
}

/*
 *  Main interpreter
 */
int main(int argc, const char* argv[])
{
    if(!util::does_file_exist(g_pwn_script_folder))
    {
        if(!util::make_file(g_pwn_script_folder))
            util::exit(EXIT_FAILURE, "Failed to create pwnscript folder!");
    }
    
    // need at least 2 args
    if(argc < 2)
        util::exit(EXIT_SUCCESS, "No script provided!");
    
    // parse the arguments
    parse_arguments(argc, argv);
    
    if(!util::does_file_exist(g_script_path))
        util::exit(EXIT_FAILURE, "Script not found!");
    
    // try to load the token list
    if(!load_token_list())
        util::exit(EXIT_FAILURE, "Failed to load token list!");
    
    // init everthing
    {
        lexer::init();
    }
    
    // maybe move this to the interpreter
    {
        // lex the main script
        lexed_script_t lexed = lexer::script(g_script_path);
        
        // parse the script
        parser::start(lexed, g_script_path);
    }
    
    // successfully finished running the script
    util::exit(EXIT_SUCCESS);
}
