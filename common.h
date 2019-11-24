/*
 *  common.h
 */
#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <stdio.h>
#include <unistd.h>

#include "definitions.h"


/*
 *  todo : move
 */


extern       std::string g_script_path;
extern const std::string g_pwn_script_folder;

extern std::unordered_map<std::string, lexed_script_t> g_lexed_scripts;

extern std::unordered_map<token_type_t, std::vector<token_t>> g_token_list;

