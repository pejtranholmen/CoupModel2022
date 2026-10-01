// FuncMetaExporter.h
#pragma once
#include "./ModelMap.h" 
#include "../ModelTypes/Func.h"
#include <ostream>
#include <filesystem>

class FuncMetaExporter
{
public:
    static bool Export(
        const ModelMap& modelMap,
        const std::filesystem::path& filename);

private:
    static void WriteHeader(std::ostream& out);

    static void WriteFunc(
        std::ostream& out,
        MyFunc myFunc,
        const Func& func);
};




