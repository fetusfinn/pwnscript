/*
 *  definitions.h
 */
#pragma once

/*
 *  Type definitions
 */

// a single byte, e.g. 8 bits
// also the byte data type
typedef unsigned char byte;

// a single line from the script
// line_t line = "type v1, v2 = value1, value2"
typedef std::string line_t;

// a single token from a line
// token_t token = "type"
typedef std::string token_t;

// a script is one file loaded line by line
// into a vector
typedef std::vector<line_t> script_t;

// a list of all tokens in a line
// vector<token_t> tokens = {"type", "v1", ",", "v2", ...}
typedef std::vector<token_t> token_list_t;

// a line that has been lexed
// a line thats been split into its tokens
typedef std::vector<token_t> lexed_line_t;

// the parts of the script that have been lexed
// a vector of vectors of tokens
typedef std::vector<lexed_line_t> lexed_script_t;

// pass c++ code to pwn script and vice verca
// typedef std::vector<variable_t> param_t;

/*
 *  defines
 */



/*
 *  enums
 */

/*
 *  Data type
 */
enum data_type_t
{
    kVoid,      // void
    kByte,      // byte
    kNumber,    // num
    kDeciml,    // dec
    kLong,      // long
    kString     // string
};


/*
 *  Type of token
 */
enum token_type_t
{
    kInvalidType = -1,
    kIdentifier = 0,// variable name, function name
    kKeyword,       // types, statements
    kSeperator,     // ",", " ", "[]"
    kOperator,      // +, -, *, /, =
    kComment,       // |, ||
    kLiteral,       // true, 3.14, "tests"
};

/*
 *  Token id / name
 *  The positions they should be in in the token list file
 */
enum token_id_t
{
    kTokenIdentifier    = 0,
    
    // keywords
    kTokenVoid          = 0,
    kTokenByte,
    kTokenNum,
    kTokenDec,
    kTokenLong,
    kTokenString,
    kTokenNull,
    kTokenUse,
    kTokenVar,
    kTokenGlobal,
    kTokenIf,
    kTokenFor,
    kTokenWhile,
    kTokenFunc,
    
    // seperators
    kTokenComma         = 0,
    kTokenCurlyOpen     = 1,
    kTokenCurlyClose    = 2,
    kTokenArray         = 3,
    kTokenBracketOpen   = 4,
    kTokenBracketClose  = 5,
    kTokenQuote         = 6,
    kTokenQuoteSingle   = 7,
    kTokenSpace         = 8,
    
    // opertors
    kTokenAnd           = 0,
    kTokenOr            = 1,
    kTokenEquals        = 2,
    kTokenNotEquals     = 3,
    kTokenPlus          = 4,
    kTokenMinus         = 5,
    kTokenMutliply      = 6,
    kTokenDivision      = 7,
    kTokenModulus       = 8,
    kTokenAssinment     = 9,
    kTokenGreaterThan   = 10,
    kTokenLessThan      = 11,
    kTokenIncrement     = 12,
    kTokenDecrement     = 13,
    kTokenObjectAccess  = 14,
    
    // comments
    kTokenCommentSingle = 0,
    kTokenCommentMulti,
};

/*
 *  structs
 */

/*
 *  A variable
 */
struct variable_t
{
    // the data type
    data_type_t m_type;
    
    // variable name as a string
    std::string m_name;
    
    // only one of these will have a value
    // the one that matches m_type
    char        m_void = NULL;
    byte        m_byte;
    int         m_num;
    float       m_dec;
    double      m_long;
    std::string m_string;
};
