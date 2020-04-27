#include "MatrixFaderFDN.h"

MatrixFaderFDN::MatrixFaderFDN (int numDelays) :
    FDN (numDelays),
    diagMat (numDelays),
    offDiagMat (numDelays),
    orthoMat (numDelays)
{
    MixingMatrixUtils::identityMatrix (diagMat);
    MixingMatrixUtils::orthonormal (orthoMat);

    for (int row = 0; row < numDelays; ++row)
        for (int col = 0; col <  numDelays; ++col)
            offDiagMat.matrix[row][col] = diagMat.matrix[row][numDelays - col];
}

void MatrixFaderFDN::updateParams()
{
    FDN::updateParams();

    auto matFade = [=] (Matrix& mat0, Matrix& mat1, float fadeAmt)
    {
        for (int row = 0; row < numDelays; ++row)
        {
            FloatVectorOperations::multiply (matrix.matrix[row], mat1.matrix[row], fadeAmt, numDelays);
            FloatVectorOperations::addWithMultiply (matrix.matrix[row], mat0.matrix[row], (1.0f - fadeAmt), numDelays);
        }
    };

    if (fader >= 0) // fade between diag and ortho
        matFade (orthoMat, diagMat, fader);
    else // fade between offDiag and ortho
        matFade (orthoMat, offDiagMat, abs (fader));
}
