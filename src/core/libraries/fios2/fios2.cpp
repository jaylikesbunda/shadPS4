#include "core/libraries/fios2/fios2.h"

#include <filesystem>
#include <cstddef>
#include "core/file_sys/fs.h"
#include "common/io_file.h"
#include "core/loader/symbols_resolver.h"
#include "common/singleton.h"
#include "common/logging/log.h"
#include "core/libraries/libs.h"

namespace Libraries::Fios2 {

s32 PS4_SYSV_ABI sceFiosInitialize(void*) {
    return 0;
}

s32 PS4_SYSV_ABI sceFiosTerminate() {
    return 0;
}

static std::filesystem::path MapGuestPath(const char* guest_path) {
    auto* mnt = Common::Singleton<Core::FileSys::MntPoints>::Instance();
    return mnt->GetHostPath(guest_path);
}

s32 PS4_SYSV_ABI sceFiosFileGetSizeSync(const char* path, u64* out_size) {
    const auto host = MapGuestPath(path);
    if (!std::filesystem::exists(host)) {
        if (out_size) *out_size = 0;
        return -1;
    }
    if (out_size) {
        *out_size = std::filesystem::file_size(host);
    }
    return 0;
}

s32 PS4_SYSV_ABI sceFiosFileReadSync(const char* path, void* buf, u64 size, u64 offset,
                                     u64* out_read) {
    const auto host = MapGuestPath(path);
    Common::FS::IOFile file(host, Common::FS::FileAccessMode::Read);
    if (!file.IsOpen()) {
        if (out_read) *out_read = 0;
        return -1;
    }
    file.Seek(static_cast<s64>(offset));
    const auto read_bytes = file.ReadRaw<std::byte>(buf, static_cast<size_t>(size));
    if (out_read) {
        *out_read = read_bytes;
    }
    return 0;
}

void RegisterlibSceFios2(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("wAKZ-det+yo", "libSceFios2", 1, "libSceFios2", 1, 0, sceFiosInitialize);
    LIB_FUNCTION("3HAgZPl1v+4", "libSceFios2", 1, "libSceFios2", 1, 0, sceFiosTerminate);
    LIB_FUNCTION("zF8-CRvRXnM", "libSceFios2", 1, "libSceFios2", 1, 0, sceFiosFileGetSizeSync);
    LIB_FUNCTION("kVMxSiYD6tc", "libSceFios2", 1, "libSceFios2", 1, 0, sceFiosFileReadSync);
}

} // namespace Libraries::Fios2 