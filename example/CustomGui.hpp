#ifndef CUSTOM_GUI_HPP
#define CUSTOM_GUI_HPP

#include "Keys.hpp"
#include "Rayex.hpp"

inline auto getRandomUniquePosition( float min, float max ) -> glm::vec3
{
  static std::vector<glm::vec3> positions;

  static std::random_device rd;
  static std::mt19937 mt( rd( ) );
  std::uniform_real_distribution<float> dist( min, max );

  glm::vec3 result = glm::vec3( 0.0F );

  while ( true )
  {
    result.x = dist( mt );
    result.y = dist( mt );
    result.z = dist( mt );

    bool accepted = true;
    for ( const auto& position : positions )
    {
      if ( result == position )
      {
        accepted = false;
      }
    }

    if ( accepted )
    {
      break;
    }
  };

  return result;
}

inline auto getRandomFloat( float min, float max ) -> float
{
  static std::random_device rd;
  static std::mt19937 mt( rd( ) );
  std::uniform_real_distribution<float> dist( min, max );

  return dist( mt );
}

inline void addBox( rx::Rayex* renderer )
{
  std::string_view path = "models/cube.obj";
  auto cube             = renderer->scene( ).findGeometry( path );
  if ( cube == nullptr )
  {
    cube = rx::loadObj( path );
    renderer->scene( ).submitGeometry( cube );
  }

  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.3F, 0.3F, 0.3F ) );
  transform      = glm::rotate( transform, getRandomFloat( 0.0F, 360.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, getRandomUniquePosition( -25.0F, 25.0F ) );

  auto cubeInstance = rx::instance( cube, transform );
  renderer->scene( ).submitGeometryInstance( cubeInstance );
}

inline void addSphere( rx::Rayex* renderer )
{
  std::string_view path = "models/sphere.obj";
  auto sphere           = renderer->scene( ).findGeometry( path );
  if ( sphere == nullptr )
  {
    sphere = rx::loadObj( path );
    renderer->scene( ).submitGeometry( sphere );
  }

  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.1F, 0.1F, 0.1F ) );
  transform      = glm::rotate( transform, getRandomFloat( 0.0F, 360.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, getRandomUniquePosition( -70.0F, 70.0F ) );

  auto sphereInstance = rx::instance( sphere, transform );
  renderer->scene( ).submitGeometryInstance( sphereInstance );
}

inline void addAwp( rx::Rayex* renderer )
{
  std::string_view path = "models/awpdlore/awpdlore.obj";
  auto awp              = renderer->scene( ).findGeometry( path );
  if ( awp == nullptr )
  {
    awp = rx::loadObj( path );
    renderer->scene( ).submitGeometry( awp );
  }

  auto transform = glm::scale( glm::mat4( 1.0F ), glm::vec3( 0.3F, 0.3F, 0.3F ) );
  transform      = glm::rotate( transform, getRandomFloat( 0.0F, 360.0F ), glm::vec3( 0.0F, 1.0F, 0.0F ) );
  transform      = glm::translate( transform, getRandomUniquePosition( -10.0F, 10.0F ) );

  auto awpInstance = rx::instance( awp, transform );
  renderer->scene( ).submitGeometryInstance( awpInstance );
}

class CustomGui : public rx::Gui
{
public:
  CustomGui( rx::Rayex* renderer ) :
    _renderer( renderer ) {}

private:
  void configure( ) override
  {
    rx::Gui::configure( );

    ImGui::StyleColorsDark( );
    ImGuiStyle& style = ImGui::GetStyle( );

    style.WindowBorderSize  = 0.0f;
    style.WindowBorderSize  = 1.0f;
    style.WindowRounding    = 0.0f;
    style.FrameRounding     = 4.0f;
    style.GrabRounding      = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 4.0f;

    ImVec4* colors = ImGui::GetStyle( ).Colors;

    colors[ImGuiCol_Text]                  = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_TextDisabled]          = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
    colors[ImGuiCol_WindowBg]              = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
    colors[ImGuiCol_ChildBg]               = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
    colors[ImGuiCol_PopupBg]               = ImVec4( 0.18f, 0.18f, 0.18f, 1.00f );
    colors[ImGuiCol_Border]                = ImVec4( 0.22f, 0.22f, 0.22f, 1.00f );
    colors[ImGuiCol_BorderShadow]          = ImVec4( 0.22f, 0.22f, 0.22f, 1.00f );
    colors[ImGuiCol_FrameBg]               = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
    colors[ImGuiCol_FrameBgHovered]        = ImVec4( 0.31f, 0.31f, 0.31f, 1.00f );
    colors[ImGuiCol_FrameBgActive]         = ImVec4( 0.41f, 0.41f, 0.41f, 0.31f );
    colors[ImGuiCol_TitleBg]               = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
    colors[ImGuiCol_TitleBgActive]         = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
    colors[ImGuiCol_MenuBarBg]             = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
    colors[ImGuiCol_ScrollbarBg]           = ImVec4( 0.21f, 0.21f, 0.21f, 1.00f );
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4( 0.31f, 0.31f, 0.31f, 1.00f );
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4( 0.41f, 0.41f, 0.41f, 1.00f );
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
    colors[ImGuiCol_CheckMark]             = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_SliderGrab]            = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ImGuiCol_SliderGrabActive]      = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ImGuiCol_Button]                = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
    colors[ImGuiCol_ButtonHovered]         = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ImGuiCol_ButtonActive]          = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ImGuiCol_Header]                = ImVec4( 0.26f, 0.26f, 0.26f, 1.00f );
    colors[ImGuiCol_HeaderHovered]         = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ImGuiCol_HeaderActive]          = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
    colors[ImGuiCol_Separator]             = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
    colors[ImGuiCol_SeparatorHovered]      = ImVec4( 0.31f, 0.31f, 0.31f, 1.00f );
    colors[ImGuiCol_SeparatorActive]       = ImVec4( 0.31f, 0.31f, 0.31f, 1.00f );
    colors[ImGuiCol_ResizeGrip]            = ImVec4( 1.00f, 1.00f, 1.00f, 0.59f );
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_ResizeGripActive]      = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_Tab]                   = ImVec4( 0.24f, 0.24f, 0.24f, 1.00f );
    colors[ImGuiCol_TabHovered]            = ImVec4( 0.27f, 0.27f, 0.27f, 1.00f );
    colors[ImGuiCol_TabActive]             = ImVec4( 0.36f, 0.35f, 0.35f, 1.00f );
    colors[ImGuiCol_TabUnfocused]          = ImVec4( 0.23f, 0.23f, 0.23f, 1.00f );
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4( 0.20f, 0.20f, 0.20f, 1.00f );
    colors[ImGuiCol_DockingPreview]        = ImVec4( 0.59f, 0.59f, 0.59f, 1.00f );
    colors[ImGuiCol_DockingEmptyBg]        = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ImGuiCol_PlotLines]             = ImVec4( 0.59f, 0.59f, 0.59f, 1.00f );
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4( 0.80f, 0.35f, 0.02f, 1.00f );
    colors[ImGuiCol_PlotHistogram]         = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
    colors[ImGuiCol_TextSelectedBg]        = ImVec4( 0.26f, 0.59f, 0.98f, 0.35f );
    colors[ImGuiCol_DragDropTarget]        = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
    colors[ImGuiCol_NavHighlight]          = ImVec4( 0.26f, 0.59f, 0.98f, 1.00f );
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
    colors[ImGuiCol_NavWindowingDimBg]     = ImVec4( 0.80f, 0.80f, 0.80f, 0.20f );
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );
  }

  void render( ) override
  {
    static bool showDemoWindow = false;
    if ( showDemoWindow )
    {
      ImGui::ShowDemoWindow( );
    }

    if ( ImGui::Begin( "Scene" ) )
    {
      if ( ImGui::Button( "Add box" ) )
      {
        addBox( _renderer );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Add sphere" ) )
      {
        addSphere( _renderer );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Add awp" ) )
      {
        addAwp( _renderer );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Clear instances" ) )
      {
        _renderer->scene( ).clearGeometryInstances( );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Clear geometry" ) )
      {
        _renderer->scene( ).clearGeometries( );
      }

      if ( ImGui::Button( "Pop geometry instance" ) )
      {
        _renderer->scene( ).popGeometryInstance( );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Pop geometry" ) )
      {
        _renderer->scene( ).popGeometry( );
      }

      if ( ImGui::Button( "Add directional light" ) )
      {
        auto directionalLight = rx::directionalLightInstance( getRandomUniquePosition( 5.0F, 10.0F ) );
        directionalLight->direction.x *= -1;

        _renderer->scene( ).submitDirectionalLight( directionalLight );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Remove directional lights" ) )
      {
        auto directionalLights = _renderer->scene( ).getDirectionalLights( );

        for ( auto directionalLight : directionalLights )
        {
          _renderer->scene( ).removeDirectionalLight( directionalLight );
        }
      }

      if ( ImGui::Button( "Set skybox" ) )
      {
        _renderer->scene( ).setEnvironmentMap( "models/skybox/cubemap_yokohama_bc3_unorm.ktx" );
      }

      ImGui::SameLine( );

      if ( ImGui::Button( "Remove skybox" ) )
      {
        _renderer->scene( ).removeEnvironmentMap( );
      }
    }

    ImGui::End( );

    if ( ImGui::Begin( "Resource Monitor" ) )
    {
      const size_t maxFrames = 10000;
      static std::array<float, maxFrames> frameTimes;

      static size_t counter = 0;
      counter               = counter % maxFrames;

      float dt = rx::Time::getDeltaTime( );
      if ( dt > 0.001f )
      {
        if ( counter >= maxFrames - 1 )
        {
          std::fill( frameTimes.begin( ), frameTimes.end( ), 0.0f );
        }

        frameTimes[counter] = dt;
        ++counter;
      }

      ImGui::PlotLines( "Frametimes", frameTimes.data( ), maxFrames, 0, "Frametime", 0.0F, 0.01F, ImVec2( 0.0F, 80.0F ) );
    }

    ImGui::End( );

    if ( ImGui::Begin( "Settings" ) )
    {
      ImGui::Checkbox( "Show ImGui demo window", &showDemoWindow );

      auto clearColor = _renderer->settings( ).getClearColor( );
      if ( ImGui::ColorEdit4( "##AmbientColor", &clearColor[0] ) )
      {
        _renderer->settings( ).setClearColor( clearColor );
      }

      int perPixelSampleRate = static_cast<int>( _renderer->settings( ).getPerPixelSampleRate( ) );
      if ( ImGui::SliderInt( "Per pixel sample rate", &perPixelSampleRate, 1, 100 ) )
      {
        _renderer->settings( ).setPerPixelSampleRate( perPixelSampleRate );
      }

      int depth = static_cast<int>( _renderer->settings( ).getRecursionDepth( ) );
      if ( ImGui::SliderInt( "Recursion depth", &depth, 0, 31 ) )
      {
        _renderer->settings( ).setRecursionDepth( static_cast<uint32_t>( depth ) );
      }

      int totalFramesToAccumulate = static_cast<int>( _renderer->settings( ).getTotalFramesToAccumulate( ) );
      if ( ImGui::SliderInt( "Frames to accumulate", &totalFramesToAccumulate, 0, 100 ) )
      {
        _renderer->settings( ).setTotalFramesToAccumulate( static_cast<uint32_t>( totalFramesToAccumulate ) );
      }
    }

    ImGui::End( );
  }

private:
  rx::Rayex* _renderer;
};

#endif // CUSTOM_GUI_HPP
