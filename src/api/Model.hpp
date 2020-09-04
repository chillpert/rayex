#ifndef MODEL_BASE_HPP
#define MODEL_BASE_HPP

#include "Texture.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "DescriptorSet.hpp"

namespace rx
{
  /// Contains all rendering properties and textures.
  class Material
  {
  public:
    Material( ) = default;
    Material( const std::string& diffuseTexture ) :
      m_diffuseTexture( { diffuseTexture }  )
    { }

    glm::vec3 m_ambient = { };
    glm::vec3 m_diffuse = { };
    glm::vec3 m_specular = { };
    glm::vec3 m_transmittance = { };
    glm::vec3 m_emission = { };

    float m_shininess = 1.0f;
    float m_reflectionIndex = 1.0f;
    float m_opacity = 1.0f;

    std::vector<std::string> m_diffuseTexture;
    std::vector<std::string> m_specularTexture;
    std::vector<std::string> m_bumpTexture;
    std::vector<std::string> m_displacementTexture;
    std::vector<std::string> m_alphaTexture;
    std::vector<std::string> m_reflectionTexture;

    /// @return Returns a set of all textures this model requires.
    std::unordered_set<std::string> getTextures( )
    {
      std::unordered_set<std::string> textures;

      if ( !m_diffuseTexture.empty( ) )
        textures.insert( m_diffuseTexture.begin( ), m_diffuseTexture.end( ) );

      if ( !m_specularTexture.empty( ) )
        textures.insert( m_specularTexture.begin( ), m_specularTexture.end( ) );

      if ( !m_bumpTexture.empty( ) )
        textures.insert( m_bumpTexture.begin( ), m_bumpTexture.end( ) );

      if ( !m_displacementTexture.empty( ) )
        textures.insert( m_displacementTexture.begin( ), m_displacementTexture.end( ) );

      if ( !m_alphaTexture.empty( ) )
        textures.insert( m_alphaTexture.begin( ), m_alphaTexture.end( ) );

      if ( !m_reflectionTexture.empty( ) )
        textures.insert( m_reflectionTexture.begin( ), m_reflectionTexture.end( ) );

      return textures;
    }
  };

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

    // Destruction through RAII
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
  /// @todo Documentation.
  template<> struct hash<rx::Model>
  {
    size_t operator()( const std::shared_ptr<rx::Model> model ) const { return hash<std::string>( )( model->m_path ); }
  };
}

#endif // MODEL_BASE_HPP