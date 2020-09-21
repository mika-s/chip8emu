#ifndef EMULATORSETTINGS_H
#define EMULATORSETTINGS_H


namespace Emu {

struct Color {
    int r, g, b, a;

    Color() = default;
    Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) { }
};

struct EmulatorSettings {
    EmulatorSettings() = default;

    EmulatorSettings(  bool isIncrementingIinFX55andFX65
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
                     , Color defaultOnColor);

    bool isIncrementingIinFX55andFX65;
    bool defaultIsIncrementingIinFX55andFX65;
    bool isVFaffectedInFX1E;
    bool defaultIsVFaffectedInFX1E;
    int frequency;
    int defaultFrequency;
    int instructionsPerFrequencyTick;
    int defaultInstructionsPerFrequencyTick;
    Color offColor;
    Color defaultOffColor;
    Color onColor;
    Color defaultOnColor;
};

}

#endif // EMULATORSETTINGS_H
