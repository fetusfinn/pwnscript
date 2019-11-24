/*
 *  module.h
 */
#pragma once

// void* cast to proper type that we get from the param_t
// typedef void*(*func_fn)(var_type_t, std::vector<param_t>);

namespace modules
{
    class c_module
    {
    private:
        
        std::string m_path;
        // std::vector<func_fn> m_funcs;
        
    public:
        
        void shutdown() {};
        
    };
}

