#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <vector>

#include <filesystem>

#include "base/base.hpp"
#include "utility/utility.hpp"

namespace ew {

class builder : public base {
public:  // methods
  builder();
  virtual auto main(int argv, char *argc[]) -> int;
  virtual auto main(std::vector<std::string> &params) -> int;
  virtual auto size() -> unsigned long { return sizeof(*this); };
  virtual auto name() -> const std::string & { return app_name; };
  virtual auto help() -> void;
  virtual auto setAppName(std::string name) -> void;

private:  // methods
  auto buildPlaylist() -> int;
  auto buildPLS() -> int;
  auto buildM3U() -> int;

private:  // vars
  std::vector<std::string> actions;

  std::string              app_name;
  std::vector<std::string> app_description;

  std::vector<std::string> file_list;
  std::filesystem::path    file;
  std::string              file_name;
  std::string              file_ext;

  std::filesystem::path    absolute_path;

  int                      number_entries = 0;
  verbosity                verbose_output = verbosity::off;
};

}  // namespace ew
#endif  // BUILDER_H
