#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <wchar.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <map>
#include <vector>
#include <thread>
#include <unistd.h>
#include "../beatsaber-hook/shared/utils/utils.h"
#include "../beatsaber-hook/shared/utils/utils-functions.h"
#include "../beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "../beatsaber-hook/shared/utils/utils-functions.h"
using namespace std;
Color newColor = { 0.0, 0.0, 0.0, 0.0 };
MAKE_HOOK_OFFSETLESS(Init, void, Il2CppObject* self, Color color, float animationDuration, Il2CppObject * rotation) {

	Init(self, newColor, 0.0, rotation);
	
}
extern "C" void load() {
	log(INFO, "Installing SpawnEffectRemover!");
	INSTALL_HOOK_OFFSETLESS(Init, il2cpp_utils::FindMethodUnsafe("", "BeatEffect", "Init", 3));
	log(INFO, "Installed!");
}