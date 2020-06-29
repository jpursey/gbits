#ifndef GBITS_RESOURCE_RESOURCE_TYPES_H_
#define GBITS_RESOURCE_RESOURCE_TYPES_H_

#include <stdint.h>

#include <string_view>
#include <tuple>

#include "gbits/base/type_info.h"

namespace gb {

class Resource;
class ResourceEntry;
class ResourceManager;
class ResourcePtrBase;
class ResourceSet;
class ResourceSystem;

template <typename Type>
class ResourcePtr;

// A resource ID is used to represent a unique ID to a resource with a given
// resource type. New resource IDs are minted by ResourceManager instance, and
// they remain unique across runs (with an vanishingly low chance of collision).
using ResourceId = uint64_t;

// A resource key is a fully unique reference to a resource. It is may be used
// as a key for sets or maps of resources.
using ResourceKey = std::tuple<TypeKey*, ResourceId>;

// Internal access token for the resource system.
class ResourceInternal final {
 public:
  ResourceInternal(const ResourceInternal&) = default;
  ResourceInternal& operator=(const ResourceInternal&) = default;
  ~ResourceInternal() = default;

 private:
  ResourceInternal() = default;

  friend class Resource;
  friend class ResourceEntry;
  friend class ResourceManager;
  friend class ResourcePtrBase;
  friend class ResourceSet;
  friend class ResourceSystem;
};

}  // namespace gb

#endif  // GBITS_RESOURCE_RESOURCE_TYPES_H_
