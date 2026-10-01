// FuncMetaExporter.cpp

#include "FuncMetaExporter.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>


namespace
{
    std::string csv_escape(std::string_view text)
    {
        bool needsQuotes = false;

        for (const char c : text) {
            if (c == ';' ||
                c == '"' ||
                c == '\n' ||
                c == '\r')
            {
                needsQuotes = true;
                break;
            }
        }

        if (!needsQuotes)
            return std::string(text);

        std::string result;
        result.reserve(text.size() + 2);

        result.push_back('"');

        for (const char c : text) {
            if (c == '"')
                result += "\"\"";
            else
                result.push_back(c);
        }

        result.push_back('"');

        return result;
    }


    std::string join_strings(
        const std::vector<std::string>& values)
    {
        std::ostringstream out;

        for (std::size_t i = 0; i < values.size(); ++i) {

            if (i > 0)
                out << '|';

            out << values[i];
        }

        return out.str();
    }

    template<typename T>
    std::string join_vector(
        const std::vector<T>& values)
    {
        std::ostringstream out;

        if constexpr (std::is_same_v<T, double>) {
            out << std::setprecision(17);
        }
        else if constexpr (std::is_same_v<T, float>) {
            out << std::setprecision(9);
        }

        for (std::size_t i = 0; i < values.size(); ++i) {

            if (i > 0)
                out << '|';

            out << values[i];
        }

        return out.str();
    }

}

void FuncMetaExporter::WriteHeader(std::ostream& out)
{
    out
        << "FuncEnum;"
        << "Name;"
        << "MyFunc;"
        << "ArgType;"
        << "SetPlotPar;"
        << "TableType;"
        << "PsParams;"
        << "PParams;"
        << "TotNumCurves;"
        << "NumCurves;"
        << "NumPoints;"
        << "LogX;"
        << "LogY;"
        << "XScalingCoef;"
        << "YScalingCoef;"
        << "XMin;"
        << "XMax;"
        << "XMaxPar;"
        << "XLabel;"
        << "YLabel;"
        << "NumX;"
        << "NumOpt;"
        << "Legends;"
        << "OptVec;"
        << "X2Vec;"
        << "X3Vec;"
        << "X4Vec;"
        << "X5Vec"
        << '\n';
}

void FuncMetaExporter::WriteFunc(
    std::ostream& out,
    MyFunc myFunc,
    const Func& func)
{
    const auto plot = func.GetPlotParam();

    const auto psNames = func.GetParDep_Ps_Names();
    const auto pNames = func.GetParDep_P_Names();

    out
        << csv_escape(func.GetName()) << ';'

        << static_cast<int>(myFunc) << ';'
        << static_cast<int>(func.GetArgumentTypes()) << ';'
        << (func.IsTableType() ? 1 : 0) << ';'

        << csv_escape(join_strings(psNames)) << ';'
        << csv_escape(join_strings(pNames)) << ';'

        << plot.Tot_Num_Of_Curves << ';'
        << plot.Num_Of_Curves << ';'
        << plot.Num_Of_Points << ';'

        << (plot.LogXScale ? 1 : 0) << ';'
        << (plot.LogYScale ? 1 : 0) << ';'

        << plot.X_ScalingCoef << ';'
        << plot.Y_ScalingCoef << ';'

        << std::setprecision(17)
        << plot.X_Min << ';'
        << plot.X_Max << ';'

        << csv_escape(plot.X_MaxPar) << ';'
        << csv_escape(plot.X_Label) << ';'
        << csv_escape(plot.Y_Label) << ';'

        << plot.Num_Of_X << ';'
        << plot.Num_Of_Opt << ';'

        << csv_escape(join_strings(plot.LegendsVec)) << ';'
        << csv_escape(join_vector(plot.Opt_Vec)) << ';'

        << csv_escape(join_vector(plot.X2_Vec)) << ';'
        << csv_escape(join_vector(plot.X3_Vec)) << ';'
        << csv_escape(join_vector(plot.X4_Vec)) << ';'
        << csv_escape(join_vector(plot.X5_Vec))

        << '\n';
}


bool FuncMetaExporter::Export(
    const ModelMap& modelMap,
    const std::filesystem::path& filename)
{
    std::ofstream out(filename);

    if (!out)
        return false;

    WriteHeader(out);

    for (const auto& [myFunc, pFunc] : modelMap.MapMyFunc) {

        if (pFunc == nullptr)
            continue;

        WriteFunc(out, myFunc, *pFunc);
    }

    return true;
}


