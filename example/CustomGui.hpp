#ifndef CUSTOM_GUI_HPP
#define CUSTOM_GUI_HPP

#include "Example.hpp"
#include "Keys.hpp"

class CustomGui : public rx::Gui
{
public:
  CustomGui( rx::Rayex* renderer ) :
    _renderer( renderer ) {}

private:
  ImFont* _font = nullptr;

  void configure( ) override
  {
    rx::Gui::configure( );

    ImGuiIO& io      = ImGui::GetIO( );
    std::string path = std::string( _renderer->settings( ).getAssetsPath( ) ) + "DroidSans.ttf";
    _font            = io.Fonts->AddFontFromFileTTF( path.c_str( ), 15.0f );

    ImGui::StyleColorsDark( );
    ImGuiStyle& style = ImGui::GetStyle( );

    style.WindowBorderSize  = 0.0f;
    style.WindowBorderSize  = 1.0f;
    style.WindowRounding    = 0.0f;
    style.FrameRounding     = 4.0f;
    style.GrabRounding      = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 4.0f;

    ImVec4* colors                         = ImGui::GetStyle( ).Colors;
    colors[ImGuiCol_Text]                  = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_TextDisabled]          = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
    colors[ImGuiCol_WindowBg]              = ImVec4( 0.18f, 0.18f, 0.18f, 0.61f );
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
    if ( _font != nullptr )
    {
      ImGui::PushFont( _font );
      ImGui::PopFont( );
    }

    static bool showDemoWindow = false;
    if ( showDemoWindow )
    {
      ImGui::ShowDemoWindow( );
    }

    static auto flags = ImGuiTreeNodeFlags_DefaultOpen;

    if ( ImGui::Begin( "Settings" ) )
    {
      if ( ImGui::CollapsingHeader( "Scene", flags ) )
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

        if ( ImGui::Button( "Set environment map" ) )
        {
          _renderer->scene( ).setEnvironmentMap( "models/skybox/cubemap_yokohama_rgba.ktx" );
        }

        ImGui::SameLine( );

        if ( ImGui::Button( "Remove environment map" ) )
        {
          _renderer->scene( ).removeEnvironmentMap( );
        }
      }

      ImGui::NewLine( );

      if ( ImGui::CollapsingHeader( "Resource Monitor", flags ) )
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

        ImGui::SetNextItemWidth( -1 );
        ImGui::PlotLines( "Frametimes", frameTimes.data( ), maxFrames, 0, "Frametime", 0.0F, 0.12F, ImVec2( 0.0F, 120.0F ) );

        std::string fpsDisplay = "FPS: " + std::to_string( rx::Time::getFramesPerSecond( ) );
        ImGui::Text( fpsDisplay.c_str( ) );
      }

      ImGui::NewLine( );

      if ( ImGui::CollapsingHeader( "Settings", flags ) )
      {
        ImGui::Checkbox( "Show ImGui demo window", &showDemoWindow );

        bool accumulateFrames = _renderer->settings( ).isAccumulatingFrames( );
        ImGui::Checkbox( "Accumulate frames", &accumulateFrames );
        _renderer->settings( ).setAccumulatingFrames( accumulateFrames );

        if ( ImGui::Button( "Recompile shaders" ) )
        {
          _renderer->settings( ).triggerPipelineRefresh( );
        }

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
      }

      ImGui::NewLine( );

      if ( ImGui::CollapsingHeader( "Scenes", flags ) )
      {
        if ( ImGui::Button( "Cornell" ) )
        {
          loadScene( _renderer, Level::eCornell );
        }

        ImGui::SameLine( );

        if ( ImGui::Button( "Debug" ) )
        {
          loadScene( _renderer, Level::eDebug );
        }

        ImGui::SameLine( );

        if ( ImGui::Button( "Spheres" ) )
        {
          loadScene( _renderer, Level::eSpheres );
        }

        ImGui::SameLine( );

        if ( ImGui::Button( "Reflections" ) )
        {
          loadScene( _renderer, Level::eReflections );
        }
      }
    }
    ImGui::End( );
  }

private:
  rx::Rayex* _renderer;
};

#endif // CUSTOM_GUI_HPP
