#include "emulatorsettings.h"


namespace Emu {

EmulatorSettings::EmulatorSettings(  bool isIncrementingIinFX55andFX65
                                   , bool defaultIsIncrementingIinFX55andFX65
                                   , bool isVFaffectedInFX1E
                                   , bool defaultIsVFaffectedInFX1E
                                   , int frequency
                                   , int defaultFrequency
                                   , int instructionsPerFrequencyTick
                                   , int defaultInstructionsPerFrequencyTick
                                   , Color offColor
                                   , Color defaultOffColor
                                   , Color onColor
                                   , Color defaultOnColor)
      : isIncrementingIinFX55andFX65(isIncrementingIinFX55andFX65)
      , defaultIsIncrementingIinFX55andFX65(defaultIsIncrementingIinFX55andFX65)
      , isVFaffectedInFX1E(isVFaffectedInFX1E)
      , defaultIsVFaffectedInFX1E(defaultIsVFaffectedInFX1E)
      , frequency(frequency)
      , defaultFrequency(defaultFrequency)
      , instructionsPerFrequencyTick(instructionsPerFrequencyTick)
      , defaultInstructionsPerFrequencyTick(defaultInstructionsPerFrequencyTick)
      , offColor(offColor)
      , defaultOffColor(defaultOffColor)
      , onColor(onColor)
      , defaultOnColor(defaultOnColor) { }
}
