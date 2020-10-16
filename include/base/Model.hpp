#ifndef MODEL_BASE_HPP
#define MODEL_BASE_HPP

#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/StorageBuffer.hpp"
#include "api/buffers/UniformBuffer.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/image/Texture.hpp"
#include "api/misc/Vertex.hpp"
#include "base/Mesh.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Describes a model.
  ///
  /// The class extracts vertex and index data from a given wavefront file.
  /// If the user wants to parse other formats than wavefront, load() should be overridden and either RAYEXEC_NAMESPACE::Renderer::pushNode() or RAYEXEC_NAMESPACE::Renderer::setNodes() templatized using the inherited class type.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// class CustomModel : public Model
  /// {
  ///   void load() override;
  /// }
  ///
  /// RayExecmyRenderer;
  /// myRenderer.init();
  ///
  /// auto cube = std::make_shared<GeometryNode>( "models/cube.obj" );
  ///
  /// myRenderer.pushNode<CustomModel>( cube );
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @ingroup Base, API
  /// @todo modelCounter is being incremented always, even if it is not a new model. This class should actually look inside Api::models or sth to assign the correct index.
  class Model
  {
  public:
    RX_API Model( );
    /// @param path The relative path to the model's file.
    RX_API Model( std::string_view path );
    RX_API virtual ~Model( ) = default;

    Model( const Model& )  = delete;
    Model( const Model&& ) = delete;

    auto operator=( const Model& ) -> Model& = delete;
    auto operator=( const Model && ) -> Model& = delete;

    /// Loads the model by extracting all vertices and indices.
    RX_API virtual void load( std::vector<Texture>& textures );

    /// @return Returns true if the model was already loaded.
    auto isLoaded( ) -> bool;

    uint32_t index = 0; ///< An index that will be used by the ray tracing structures to access the correct model.
    std::string path;   ///< The relative path to the model.
    Material material;  ///< Holds all rendering information including the textures required by the model.
    bool initialized = false;

    std::vector<Vertex> vertices;  ///< The raw vertex data.
    std::vector<uint32_t> indices; ///< The raw index data.

    // Destruction via RAII.
    VertexBuffer vertexBuffer; ///< Used to buffer the raw vertex data.
    IndexBuffer indexBuffer;   ///< Used to buffer the raw index data.

    std::vector<Mesh> meshes;
    StorageBuffer meshDataBuffer;

  private:
    static uint32_t modelCounter; ///< Used to count the total number of models and to assign a new unique value to index.
  };
} // namespace RAYEXEC_NAMESPACE

namespace std
{
  /// @cond INTERNAL
  template <>
  struct hash<RAYEXEC_NAMESPACE::Model>
  {
    auto operator( )( const shared_ptr<RAYEXEC_NAMESPACE::Model>& model ) const -> size_t { return hash<string>( )( model->path ); }
  };
  /// @endcond
} // namespace std

#endif // MODEL_BASE_HPP
