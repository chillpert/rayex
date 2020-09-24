#ifndef STDAFX_HPP
#define STDAFX_HPP

#include "Core.hpp"
#include "base/Time.hpp"
#include "external/ImGui/imgui.h"
#include "external/ImGui/imgui_impl_sdl.h"
#include "external/ImGui/imgui_impl_vulkan.h"

#define GLFORCE_DEPTH_ZERO_TO_ONE
#include <external/glm/glm.hpp>
#include <external/glm/ext/matrix_transform.hpp>
#include <external/glm/ext/matrix_clip_space.hpp>
#include <external/glm/gtx/string_cast.hpp>

#include <vector>
#include <optional>
#include <set>
#include <algorithm>
#include <map>
#include <fstream>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <any>
#include <filesystem>

#endif // STDAFX_HPP