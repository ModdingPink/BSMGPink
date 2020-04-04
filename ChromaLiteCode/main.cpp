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
using namespace il2cpp_utils; //so we dont have to constantly type 'il2cpp_utils::RunMethod, its just RunMethod'
float getValue(int rgb, int shift) {
	int value = (rgb >> shift) & 0x0ff; //Shitfs the bits 'shift' amounts of times to the right and only looks at the last two bytes. eg a number which maxes to 255 
	float floatValue = (value / 255.0); //make a float which a colour can use, a number between 0 and 1 hense we devide by 255.0
	return (floatValue);
}
MAKE_HOOK_OFFSETLESS(HandleBeatmapObject, void, Il2CppObject* self, Il2CppObject* beatmapEventData) {
	int value;
	GetFieldValue(&value, beatmapEventData, "value"); //Chroma works of the beatmaps value, so we get this value first
	if (value > 1999999999) { //Chroma Events only occur with values of 2000000000 and above 
		//log(DEBUG, "Chroma Event!");
		value = value - 2000000000; 
		float red = getValue(value, 16); //gets the Red Value, the first byte of the number 'value' by shifting the bits 16 across
		float green = getValue(value, 8); //gets the Green Value, the second byte of the number 'value'
		float blue = getValue(value, 0); //gets the Blue Value, the last byte of the number 'value' 
		float alpha = 1.0; //Opaque
		//log(DEBUG, "Red :%f", red);
		//log(DEBUG, "Green :%f", green);
		//log(DEBUG, "Blue :%f", blue);
		Color newColor = {red, green, blue, alpha}; //create a new color with values created from the beatmaps value
		Il2CppObject* lightColor; //create a new Il2CppObject which is unspecified.
		if (!RunMethod(&lightColor, il2cpp_utils::GetClassFromName("UnityEngine", "ScriptableObject"), "CreateInstance", GetSystemType("", "SimpleColorSO"))) {
			log(DEBUG, "Failed to create lightColor"); //RunMethods return false if it errors. 
		} //Make it an empty SimpleColorSO
		if (!RunMethod(lightColor, "SetColor", newColor)) { //Set the colour of that ColorSO
			log(DEBUG, "Failed to set lightColor"); //RunMethods return false if it errors. 
		}
		//We have to create a new SimpleColorSO as ColorSO does not have a 'SetColor' Method, but SimpleColorSO does.
		//There is no easy way to specify the ColorSO to be a SimpleColorSO, so we create a new one.
		SetFieldValue(self, "_lightColor0", lightColor); //set the lights to this new colour
		SetFieldValue(self, "_lightColor1", lightColor);
		SetFieldValue(self, "_highlightColor0", lightColor);
		SetFieldValue(self, "_highlightColor1", lightColor);
	}
	//else {
	//	log(DEBUG, "Not A Chroma Event! %d", value);
	//}
	HandleBeatmapObject(self, beatmapEventData); //Run the games code
}

extern "C" void load() {

	log(INFO, "Installing Chromalite!");
	INSTALL_HOOK_OFFSETLESS(HandleBeatmapObject, FindMethodUnsafe("", "LightSwitchEventEffect", "HandleBeatmapObjectCallbackControllerBeatmapEventDidTrigger", 1)); //Hook onto the lighting events
	log(INFO, "Installed!");
}

