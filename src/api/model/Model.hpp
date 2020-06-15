#ifndef MODEL_HPP
#define MODEL_HPP

#include "Base.hpp"

namespace RX
{
  class Model : public ModelBase
  {
  public:
    RX_API Model(const std::string& path);
    RX_API ~Model();

    // Fill the vectors with vertex and index data.
    RX_API virtual void initialize();

    //inline UniformBufferObject getUbo() { return { m_model, m_view, m_projection, m_cameraPos }; }

    

    glm::mat4 m_model = glm::mat4(1.0f);
    glm::mat4 m_inversedModel = glm::mat4(1.0f);
    //glm::mat4 m_view = glm::mat4(1.0f);

    //glm::mat4 m_projection = glm::mat4(1.0f);
    //glm::vec3 m_cameraPos = glm::vec3(1.0f);

    uint32_t m_objIndex = 0;
    uint32_t txtOffset = 0; // Offset in 'm_textures'

    std::vector<Material> m_materials; // This probably requires keeping track of submeshes.
  };
}

#endif // MODEL_HPP