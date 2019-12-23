/// \file     StringTable.h
/// \brief    List of strings
///           HOW TO ADD STRING A STRING
///           1. Add an ID to the StringTableIDTy
///           2. Add the string pair in the StringTable array
///           3. IMPORTANT! The number of pairs must match the IDs up to CUSTOM_ID
/// \author   Nicolas Bourré
///

#ifndef STRINGTABLE_H
#define STRINGTABLE_H


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define STRINGTABLE_CUSTOM_MAX    150


////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////
///
/// \enum   StringTableLangTy
/// \brief  Supported languages.
///
/// Identifies the different supported system languages.
typedef enum
{
  STRINGTABLE_LANG_EN   = 0,    ///< English.
  STRINGTABLE_LANG_FR,        ///< French.
  
  STRINGTABLE_LANG_MAX        ///< Maximum number of supported languages.
} StringTableLangTy;

///
/// \enum   StringTableIDTy
/// \brief  The string (ID) table.
///
/// Identifies the different string IDs.
typedef enum {
  STRINGTABLE_ID_000_EMPTY    = 0,
  STRINGTABLE_ID_001_HEADER_TIME,
  STRINGTABLE_ID_002_HEADER_TEMP,
  STRINGTABLE_ID_003_HEADER_TEMP_MAX,
  STRINGTABLE_ID_004_HEADER_TEMP_MIN,
  STRINGTABLE_ID_005_HEADER_TEMP_AVG,
  
  STRINGTABLE_ID_999_CUSTOM,        ///< Custom string. Used to allow precise formatting in specific contexts.
  
  STRINGTABLE_ID_MAX,               ///< Maximum number of supported strings.
  STRINGTABLE_ID_NONE,              ///< String ID for no string. 
} StringTableIDTy;

static const char *StringTable[STRINGTABLE_ID_MAX][STRINGTABLE_LANG_MAX] = {
  {"", ""},                                // STRINGTABLE_ID_000_EMPTY
  {"Time", "Temps"},                       // STRINGTABLE_ID_001_HEADER_TIME
  {"Temperature", "Temperature"},          // STRINGTABLE_ID_002_HEADER_TEMP
  {"Temp. max", "Temp. max"},              // STRINGTABLE_ID_003_HEADER_TEMP_MAX
  {"Temp. min", "Temp. min"},              // STRINGTABLE_ID_004_HEADER_TEMP_MIN
  {"Temp. avg", "Temp. avg"},              // STRINGTABLE_ID_005_HEADER_TEMP_AVG

  {"", ""},                      // STRINGTABLE_ID_999_CUSTOM
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void     StringTableSetLang(StringTableLangTy lang);
const char* StringTableGetStr(StringTableIDTy strID);
const char* StringTableGetStrInLang(StringTableLangTy lang, StringTableIDTy strID);
char*     StringTableGetCustomStr();

#endif
