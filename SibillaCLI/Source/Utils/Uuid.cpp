#include "Utils/Uuid.h"

#include <absl/random/random.h>
#include <absl//strings/str_format.h>

namespace scli
{
    std::string GenerateUUID()
    {
        absl::BitGen bitgen;  // Random number generator
      
        // Generate random parts of the UUID
        uint32_t time_low = absl::Uniform<uint32_t>(bitgen, 0, 0xFFFFFFFF);
        uint16_t time_mid = absl::Uniform(bitgen, 0, 0xFFFF);
        uint16_t time_hi_and_version = absl::Uniform(bitgen, 0, 0x0FFF) | (4 << 12); // Version 4 UUID
        uint16_t clock_seq = absl::Uniform(bitgen, 0, 0x3FFF) | 0x8000;             // Variant 1 UUID
        uint64_t node = absl::Uniform(bitgen, 0, 0xFFFFFFFFFFFF);
      
        // Format the UUID as a string
        return absl::StrFormat
        (
            "%08x-%04x-%04x-%04x-%012x", 
            time_low, 
            time_mid, 
            time_hi_and_version, 
            clock_seq, 
            node
        );
      }
}