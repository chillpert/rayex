#ifndef BASE_HPP
#define BASE_HPP

// This file contains all programming interfaces required by the library. 
// All of the client's own implementations need be derived from these classes.
//
// The following APIs exist:
//   - CameraBase
//   - TransformNode
//   - GeometryNode
//   - LightNode
//   - DirectionalLightNode
//   - GuiBase
//   - WindowBase

#include "base/CameraBase.hpp"
#include "base/NodeBase.hpp"
#include "base/LightNodeBase.hpp"
#include "base/GeometryNodeBase.hpp"
#include "base/GuiBase.hpp"
#include "window/WindowBase.hpp"
#include "Settings.hpp"

#endif // BASE_HPP