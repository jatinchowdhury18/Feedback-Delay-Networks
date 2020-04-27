#ifndef MATRIXFADERFDN_H_INCLUDED
#define MATRIXFADERFDN_H_INCLUDED

#include "FDN.h"

class MatrixFaderFDN : public FDN
{
public:
    MatrixFaderFDN (int numDelays);

    void setFade (float newFade) { fader = newFade; }

    void updateParams() override;

private:
    float fader = 0.0f; // -1 to 1

    Matrix diagMat;
    Matrix offDiagMat;
    Matrix orthoMat;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MatrixFaderFDN)
};

#endif // MATRIXFADERFDN_H_INCLUDED
