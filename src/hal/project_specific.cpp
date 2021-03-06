#include "daScript/misc/platform.h"
#include "daScript/daScriptModule.h"
#include "daScript/simulate/fs_file_info.h"

using namespace das;

void require_project_specific_modules() {
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_UriParser);
    NEED_MODULE(Module_PathTracerHelper);
    NEED_MODULE(Module_TestProfile);
    NEED_MODULE(Module_UnitTest);
}

smart_ptr<das::FileAccess> get_file_access() {
    return make_smart<FsFileAccess>();
}

