#ifndef STDAFX_HPP
#define STDAFX_HPP

#include "Core.hpp"
#include "base/Time.hpp"
#include "external/ImGui/imgui.h"
#include "external/ImGui/imgui_impl_sdl.h"
#include "external/ImGui/imgui_impl_vulkan.h"

#include <gsl/gsl>

#define GLFORCE_DEPTH_ZERO_TO_ONE
#include <algorithm>
#include <any>
#include <array>
#include <external/glm/ext/matrix_clip_space.hpp>
#include <external/glm/ext/matrix_transform.hpp>
#include <external/glm/glm.hpp>
#include <external/glm/gtx/string_cast.hpp>
#include <filesystem>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#endif // STDAFX_HPP