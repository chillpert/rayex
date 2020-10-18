#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include "api/Vertex.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// Describes the rendering properties of a mesh.
  struct Material
  {
    glm::vec4 ambient  = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 ambient  + vec1 texture index
    glm::vec4 diffuse  = glm::vec4( 0.2F, 1.0F, 1.0F, -1.0F ); // vec3 diffuse  + vec1 texture index
    glm::vec4 specular = glm::vec4( 1.0F, 1.0F, 1.0F, -1.0F ); // vec3 specular + vec1 texture index
  };

  /// Describes a sub-mesh and its material.
  /// @warning If indexOffset is not set correctly the mesh can not be displayed properly. Take a look at loadObj(std::string_view) for a working example.
  struct Mesh
  {
    Material material = { }; ///< The mesh's material.

  private:
    glm::vec4 padding0 = glm::vec4( 1.0F ); ///< Buffer padding (ignore).
    glm::vec4 padding1 = glm::vec4( 1.0F ); ///< Buffer padding (ignore).
    glm::vec4 padding2 = glm::vec4( 1.0F ); ///< Buffer padding (ignore).
    uint32_t padding3  = 0;                 ///< Buffer padding (ignore).
    uint32_t padding4  = 0;                 ///< Buffer padding (ignore).
    uint32_t padding5  = 0;                 ///< Buffer padding (ignore).

  public:
    uint32_t indexOffset = 0; ///< Refers to the offset of this mesh inside a Geometry::indices container.
  };

  /// Describes a geometry RayExec can render.
  ///
  /// Even if a model consists out of multiple sub-meshes, all vertices and indices must be stored together in their respective containers.
  /// @warning geometryIndex must be incremented everytime a new model is created.
  struct Geometry
  {
    std::vector<Vertex> vertices;  ///< Contains all vertices of the geometry.
    std::vector<uint32_t> indices; ///< Contains all indices of the geometry.
    std::vector<Mesh> meshes;      ///< Contains all sub-meshes and their respective materials.
    uint32_t geometryIndex = 0;    ///< A unique index required by the acceleration structures.
    std::string_view path;         ///< The model's path, relative to the path to assets.
  };

  /// Describes an instance of some geometry.
  /// @warning To assign a specific geometry to an instance, both must have the same value for geometryIndex.
  struct GeometryInstance
  {
    glm::mat4 transform    = glm::mat4( 1.0F ); ///< The instance's world transform matrix.
    glm::mat4 transformIT  = glm::mat4( 1.0F ); ///< The inverse transpose of transform.
    uint32_t geometryIndex = 0;                 ///< Used to assign this instance a model.

  private:
    float padding0 = 0.0F; ///< Buffer padding (ignore).
    float padding1 = 0.0F; ///< Buffer padding (ignore).
    float padding2 = 0.0F; ///< Buffer padding (ignore).
  };

  /// A common base for all light types.
  ///
  /// Instances of this type should not be used anywhere.
  /// @ingroup BASE
  struct Light
  {
    glm::vec3 ambient  = glm::vec3( 1.0F ); ///< The ambient color.
    glm::vec3 diffuse  = glm::vec3( 1.0F ); ///< The diffuse color.
    glm::vec3 specular = glm::vec3( 1.0F ); ///< The specular color.

    float ambientIntensity  = 0.0F; ///< The intensity of the ambient color.
    float diffuseIntensity  = 0.0F; ///< The intensity of the diffuse color.
    float specularIntensity = 0.0F; ///< The intensity of the specular color.
  };

  /// A directional light.
  /// @ingroup BASE
  struct DirectionalLight : public Light
  {
    glm::vec3 direction = glm::vec3( 1.0F );
  };

  /// A point light.
  /// @ingroup BASE
  struct PointLight : public Light
  {
    glm::vec3 position = glm::vec3( 1.0F );

    float constant  = 0.0F;
    float linear    = 0.0F;
    float quadratic = 0.0F;
  };

  /// A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.
  ///
  /// The function will attempt to find sub-meshes in the file and retrieve all materials.
  /// A user is encouraged to create their own model loader function or classes.
  /// @path The model's path, relative to the path to assets.
  /// @return Returns a pointer to a geometry object.
  RX_API std::shared_ptr<Geometry> loadObj( std::string_view path );

  /// A commodity function for allocating an instance from a given geometry and set its matrices.
  ///
  /// The function will also automatically set the inverse transpose matrix.
  /// @param geometry The geometry to create the instance from.
  /// @param transform The world transform matrix of the instance.
  /// @return Returns a pointer to a geometry instance.
  RX_API std::shared_ptr<GeometryInstance> instance( std::shared_ptr<Geometry> geometry, const glm::mat4& transform = glm::mat4( 1.0F ) );

} // namespace RAYEXEC_NAMESPACE

#endif // INTERFACES_HPP
