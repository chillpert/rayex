#ifndef BASE_HPP
#define BASE_HPP

/* 
  This file contains all programming interfaces required by the library. 
  All of the client's own implementations need be derived from these classes.

  The following APIs exist:
    - CameraBase
    - ModelBase
    - TransformNodeBase
    - GeometryNodeBase
    - GuiBase
    - WindowBase

  You can use the default implementations. 
  But for a custom one, simply inherit from any of these classes.
*/

#include "CameraBase.hpp"
#include "NodeBase.hpp"
#include "ModelBase.hpp"
#include "GuiBase.hpp"
#include "WindowBase.hpp"

#endif // BASE_HPP