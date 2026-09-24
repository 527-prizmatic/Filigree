#ifndef FILIGREE_FILEPATH_FUNCS_HPP_
#define FILIGREE_FILEPATH_FUNCS_HPP_

#include <libraries.hpp>

namespace filigree {
    /// maxLength specifies how many directories the path can have before it is shortened.
    /// The last directory and the filename are always included in the shortened path.
    /// If the path is shorter than maxLength, it is returned as-is.
    /// A shortened path begins with an ellipsis.
    std::string shortenPath(std::filesystem::path& path, size_t maxLength) {
        std::string shortened { };
        std::string separator { " / " };

        if (path.has_relative_path()) {
            std::vector<std::string> dirs { };
            for (auto& dir : path.parent_path()) {
                dirs.push_back(dir.string());
            }

            if (dirs.size() > maxLength) {
                shortened += "..." + separator;
                for (size_t i = dirs.size() - maxLength; i < dirs.size(); i++) {
                    shortened += dirs[i] + separator;
                }
            }
            else {
                for (auto& dir : dirs) {
                    shortened += dir + separator;
                }
            }
        }

        shortened += path.filename().string();

        return shortened;
    }
}

#endif // FILIGREE_FILEPATH_FUNCS_HPP_