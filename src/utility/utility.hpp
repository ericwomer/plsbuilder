

#include <ostream>
#include <type_traits>
#include <filesystem>

namespace ew {

enum class verbosity { off = 0, info = 1, developer = 2 };
enum class format { undefined = 0, pls = 1, m3u = 2 };

inline auto operator<<(std::ostream &os, const verbosity &obj) -> std::ostream &
{
  os << static_cast<std::underlying_type<verbosity>::type>(obj);
  return os;
}

inline auto operator<<(std::ostream &os, const format &obj) -> std::ostream &
{
  os << static_cast<std::underlying_type<format>::type>(obj);
  return os;
}

// Helper methods
auto writeFile(std::filesystem::path filename, std::string &contents, verbosity verbose_output = verbosity::off) -> int;
}  // namespace ew