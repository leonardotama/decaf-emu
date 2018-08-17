#pragma once
#include <common/decaf_assert.h>
#include <libcpu/mem.h>
#include <limits>
#include <vector>
#include <map>

namespace kernel
{

namespace loader
{

// Reason codes for RPL entrypoint invocation
static const int
RplEntryReasonLoad = 1;

static const int
RplEntryReasonUnload = 2;

struct LoadedModule;

struct LoadedModuleHandleData
{
   LoadedModule *ptr;
};

enum class LoadedSectionType
{
   Unknown,
   Code,
   Data
};

struct LoadedSection
{
   std::string name;
   LoadedSectionType type;
   ppcaddr_t start;
   ppcaddr_t end;
};

enum class SymbolType
{
   Unknown,
   Data,
   Function,
   TLS
};

struct Symbol
{
   ppcaddr_t address;
   SymbolType type;
};

struct LoadedModule
{
   ppcaddr_t
   findExport(const std::string& symName) const
   {
      auto itr = exports.find(symName);

      if (itr == exports.end()) {
         return 0u;
      }

      return itr->second;
   }

   Symbol *
   findSymbol(ppcaddr_t address)
   {
      for (auto &sym : symbols) {
         if (sym.second.address == address) {
            return &sym.second;
         }
      }

      return nullptr;
   }

   template<typename Type>
   Type *
   findDataExport(const std::string& varName) const
   {
      return reinterpret_cast<Type *>(mem::translate(findExport(varName)));
   }

   LoadedSection *
   findAddressSection(ppcaddr_t address)
   {
      for (auto &sec : sections) {
         if (address >= sec.start && address < sec.end) {
            return &sec;
         }
      }

      return nullptr;
   }

   std::string name;
   LoadedModuleHandleData *handle = nullptr;
   ppcaddr_t entryPoint = 0;
   uint32_t defaultStackSize = 0;
   ppcaddr_t sdaBase = 0;
   ppcaddr_t sda2Base = 0;
   uint32_t tlsModuleIndex = std::numeric_limits<uint32_t>::max();
   ppcaddr_t tlsBase = 0;
   uint32_t tlsAlignShift = 0;
   uint32_t tlsSize = 0;
   bool entryCalled = false;
   std::vector<LoadedSection> sections;
   std::map<std::string, ppcaddr_t> exports;
   std::map<std::string, Symbol> symbols;
};

static inline LoadedSection *
findSectionForAddress(ppcaddr_t address)
{
   return nullptr;
}

static inline std::string *
findSymbolNameForAddress(ppcaddr_t address)
{
   return nullptr;
}

static inline std::string
findNearestSymbolNameForAddress(ppcaddr_t address)
{
   return "fuk u";
}

} // namespace loader

} // namespace kernel
