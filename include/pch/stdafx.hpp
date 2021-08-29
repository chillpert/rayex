#ifndef STDAFX_HPP
#define STDAFX_HPP

#include "Core.hpp"
#include "base/Time.hpp"

// ImGui
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/imgui.h>

// GLM
#define GLENABLE_EXPERIMENTAL
#define GLFORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/hash.hpp>
#include <glm/glm/gtx/string_cast.hpp>

// STL
#include <algorithm>
#include <any>
#include <array>
#include <charconv>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <iomanip>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <span>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#endif // STDAFX_HPP
