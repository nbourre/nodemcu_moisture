///
/// \file     StringTable.c
/// \brief    List of strings
/// \author   NOVO - Dany Thiffeault, ing. (134853)
///

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "StringTable.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
static    StringTableLangTy g_stringTableLang = STRINGTABLE_LANG_EN;        ///< Configured system language.
static char g_customString[STRINGTABLE_LANG_MAX][STRINGTABLE_CUSTOM_MAX];     ///< Custom string.


////////////////////////////////////////////////////////////////////////////////
/// \brief    StringTableSetLang - Set system language.
/// \public
///
/// \param[in]  lang  Language to use in the string table.
////////////////////////////////////////////////////////////////////////////////
void StringTableSetLang(StringTableLangTy lang)
{
  g_stringTableLang = lang;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief    StringTableGetStr - String retrieval function from table.
/// \public
///
/// \param[in]  strID ID of the string to retrieve.
///
/// \return   Pointer to the string.
////////////////////////////////////////////////////////////////////////////////
const char* StringTableGetStr(StringTableIDTy strID)
{
  return StringTable[strID][g_stringTableLang];
}

////////////////////////////////////////////////////////////////////////////////
/// \brief    StringTableGetStrInLang - String retrieval function from table in a specific language.
/// \public
///
/// \param[in]  lang  Langage of the string to retrieve.
/// \param[in]  strID ID of the string to retrieve.
///
/// \return   Pointer to the string.
////////////////////////////////////////////////////////////////////////////////
const char* StringTableGetStrInLang(StringTableLangTy lang, StringTableIDTy strID)
{
  return StringTable[strID][lang];
}

////////////////////////////////////////////////////////////////////////////////
/// \brief    StringTableGetCustomStr - String retrieval function from custom
///                     strings table.
/// \public
///
/// \return   Pointer to the string.
////////////////////////////////////////////////////////////////////////////////
char* StringTableGetCustomStr()
{
  return g_customString[g_stringTableLang];
}