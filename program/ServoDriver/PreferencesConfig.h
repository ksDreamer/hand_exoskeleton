#include <Preferences.h>

Preferences preferences;


void preferencesSetup(){
  preferences.begin("Tasks", false);
}
