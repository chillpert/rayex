#ifndef BINDING_HPP
#define BINDING_HPP

#include "api/utility/Helpers.hpp"
#include "api/utility/Initializers.hpp"

namespace RAYEX_NAMESPACE
{
  /// A utility class for managing descriptor related resources.
  /// @ingroup API
  class Bindings
  {
  public:
    /// Used to add a binding.
    /// @param binding The binding's index.
    /// @param type The binding's descriptor type.
    /// @param stage The binding's shader type.
    /// @param count The amount of descriptors for this binding.
    /// @param flags The binding's descriptor binding flags.
    void add( uint32_t binding, vk::DescriptorType type, vk::ShaderStageFlags stage, uint32_t count = 1, vk::DescriptorBindingFlags flags = { } );

    /// Used to initialize a unique descriptor set layout.
    /// @return Returns a descriptor set layout with a unqiue handle.
    auto initLayoutUnique( vk::DescriptorSetLayoutCreateFlags flags = { } ) -> vk::UniqueDescriptorSetLayout;

    /// Used to initialize a descriptor pool.
    /// @param maxSets The maximum amount of descriptor sets that can be allocated from the pool.
    /// @param flags The pool's create flags.
    /// @return Returns a descriptor pool with a unique handle.
    auto initPoolUnique( uint32_t maxSets, vk::DescriptorPoolCreateFlags flags = { } ) -> vk::UniqueDescriptorPool;

    /// Updates the descriptor set.
    /// @note There are no descriptor set handles required for this function.
    void update( );

    /// Used to create a descriptor write for an acceleration structure.
    /// @param sets The descriptor set handles.
    /// @param binding The binding's index.
    /// @param pWriteDescriptorSetAccelerationStructureKHR A pointer to a acceleration structure descriptor write.
    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::WriteDescriptorSetAccelerationStructureKHR* pWriteDescriptorSetAccelerationStructureKHR );

    /// Used to create a descriptor write for an image.
    /// @param sets The descriptor set handles.
    /// @param binding The binding's index.
    /// @param pImageInfo A pointer to an image descriptor write.
    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorImageInfo* pImageInfo );

    /// Used to create a descriptor write for a buffer.
    /// @param sets The descriptor set handles.
    /// @param binding The binding's index.
    /// @param pBufferInfo A pointer to a buffer descriptor write.
    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorBufferInfo* pBufferInfo );

    /// Used to create a descriptor write for uniform buffers.
    /// @param sets The descriptor set handles.
    /// @param binding The binding's index.
    /// @param uniformBufferInfos The uniform buffer's descriptor buffer infos.
    void write( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, const std::vector<vk::DescriptorBufferInfo>& uniformBufferInfos );

    /// Used to create an array of descriptor writes for buffers.
    /// @param sets The descriptor set handles.
    /// @param binding The binding's index.
    /// @param pBufferInfo The pointer to the first element of an array of descriptor buffer infos.
    void writeArray( const std::vector<vk::DescriptorSet>& sets, uint32_t binding, vk::DescriptorBufferInfo* pBufferInfo );

    /// Used to set pool sizes manually.
    /// @param poolSizes The pool sizes to set.
    /// @note Must be set before initializing the descriptor pool.
    void setPoolSizes( const std::vector<vk::DescriptorPoolSize>& poolSizes );

    /// Resets all members.
    void reset( );

  private:
    /// Used to create a descriptor write.
    /// @param set The descriptor set to bind to.
    /// @param writeIndex The write index of the descriptor set.
    /// @param binding The binding's index.
    /// @return Returns the index of the matching binding.
    auto write( vk::DescriptorSet set, size_t writeIndex, uint32_t binding ) -> size_t;

    /// Used to create an array of descriptor writes.
    /// @param set The descriptor set to bind to.
    /// @param writeIndex The write index of the descriptor set.
    /// @param binding The binding's index.
    /// @return Returns the index of the matching binding.
    auto writeArray( vk::DescriptorSet set, size_t writeIndex, uint32_t binding ) -> size_t;

    std::vector<vk::DescriptorSetLayoutBinding> _bindings;         ///< Contains the actual binding.
    std::vector<vk::DescriptorBindingFlags> _flags;                ///< Contains binding flags for each of the actual bindings.
    std::optional<std::vector<vk::DescriptorPoolSize>> _poolSizes; ///< The pool sizes for allocating the descriptor pool (Only used if setPoolSizes(const std::vector<vk::DescriptorPoolSize>&) is called).
    std::vector<std::vector<vk::WriteDescriptorSet>> _writes;      ///< Contains all descriptor writes for each binding.
  };

  /// Encapsulates descriptor-related resources.
  /// @ingroup API
  struct Descriptors
  {
    vk::UniqueDescriptorSetLayout layout;
    vk::UniqueDescriptorPool pool;
    Bindings bindings;
  };

} // namespace RAYEX_NAMESPACE

#endif // BINDING_HPP
