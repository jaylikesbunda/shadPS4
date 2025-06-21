#pragma once

#include "common/types.h"

namespace Core::Loader {
class SymbolsResolver;
}

namespace Libraries::Fios2 {

s32 PS4_SYSV_ABI sceFiosInitialize(void* param);
s32 PS4_SYSV_ABI sceFiosTerminate();

s32 PS4_SYSV_ABI sceFiosFileGetSizeSync(const char* path, u64* out_size);
s32 PS4_SYSV_ABI sceFiosFileReadSync(const char* path, void* buf, u64 size, u64 offset,
                                     u64* out_read);

void RegisterlibSceFios2(Core::Loader::SymbolsResolver* sym);

} // namespace Libraries::Fios2 