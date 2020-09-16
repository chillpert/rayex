#ifndef MODEL_BASE_HPP
#define MODEL_BASE_HPP

#include "api/image/Texture.hpp"
#include "api/misc/Vertex.hpp"
#include "api/buffers/VertexBuffer.hpp"
#include "api/buffers/IndexBuffer.hpp"
#include "api/buffers/UniformBuffer.hpp"
#include "api/descriptors/DescriptorSet.hpp"
#include "api/Material.hpp"

namespace RENDERER_NAMESPACE
{
  /// Describes a model.
  /// 
  /// The class extracts vertex and index data from a given wavefront file.
  /// If the client wants to parse other formats than wavefront, load() should be overridden and either rx::Renderer::pushNode() or rx::Renderer::setNodes() templatized using the inherited class type.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// class CustomModel : public Model
  /// {
  ///   void load() override;
  /// }
  /// 
  /// Renderer myRenderer;
  /// myRenderer.init();
  /// 
  /// auto cube = std::make_shared<GeometryNode>( "models/cube.obj", Material( "textures/metal.png" ) );
  /// 
  /// myRenderer.pushNode<CustomModel>( cube );
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @ingroup Base, API
  /// @todo m_modelCounter is being incremented always, even if it is not a new model. This class should actually look inside Api::m_models or sth to assign the correct index.
  class Model
  {
  public:
    RX_API Model( );
    /// @param path The relative path to the model's file.
    RX_API Model( const std::string& path );
    RX_API virtual ~Model( ) = default;

    /// Loads the model by extracting all vertices and indices.
    RX_API virtual void load( );

    /// @return Returns true if the model was already loaded.
    bool isLoaded( );

    uint32_t m_index = 0; ///< An index that will be used by the ray tracing structures to access the correct model.
    std::string m_path; ///< The relative path to the model.
    Material m_material; ///< Holds all rendering information including the textures required by the model.
    bool m_initialized = false;

    std::vector<Vertex> m_vertices; ///< The raw vertex data.
    std::vector<uint32_t> m_indices; ///< The raw index data.

    // Destruction via RAII.
    VertexBuffer m_vertexBuffer; ///< Used to buffer the raw vertex data.
    IndexBuffer<uint32_t> m_indexBuffer; ///< Used to buffer the raw index data.

    // No destruction necessary
    DescriptorSet m_descriptorSets; ///< Descriptor sets for the model's descriptors.

  private:
    static uint32_t m_modelCounter; ///< Used to count the total number of models and to assign a new unique value to m_index.
  };
}

namespace std
{
  /// @cond INTERNAL
  template<> struct hash<rx::Model>
  {
    size_t operator()( const std::shared_ptr<rx::Model> model ) const { return hash<std::string>( )( model->m_path ); }
  };
  /// @endcond
}

#endif // MODEL_BASE_HPP