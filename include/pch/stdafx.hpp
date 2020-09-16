#ifndef STDAFX_HPP
#define STDAFX_HPP

#include "Core.hpp"
#include "window/Time.hpp"
#include "external/ImGui/imgui.h"
#include "external/ImGui/imgui_impl_sdl.h"
#include "external/ImGui/imgui_impl_vulkan.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <external/glm/glm.hpp>
#include <external/glm/ext/matrix_transform.hpp>
#include <external/glm/ext/matrix_clip_space.hpp>
#include <external/glm/gtx/string_cast.hpp>

#include <iomanip>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <optional>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <fstream>
#include <array>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#endif // STDAFX_HPP