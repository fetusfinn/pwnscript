/*
 *  lexer.h
 */
#pragma once

namespace lexer
{
    void init();
    lexed_script_t script(const std::string& script_path, bool relaod = false);
}
