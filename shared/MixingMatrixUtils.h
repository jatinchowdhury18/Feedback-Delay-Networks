#ifndef MIXING_MATRIX_UTILS_H_INCLUDED
#define MIXING_MATRIX_UTILS_H_INCLUDED

#include "JuceHeader.h"
#include "r8lib/matrix_exponential.h"

/** Class for FDN mixing matrices */
struct Matrix
{
    Matrix (int dim) :
        dim (dim)
    {
        // Allocate memory...
        matrix = new float*[dim];
        for (int i = 0; i < dim; ++i)
            matrix[i] = new float[dim];
    }

    Matrix (const Matrix& oldMatrix) :
        Matrix (oldMatrix.dim)
    {
        for (int row = 0; row < dim; ++row)
            FloatVectorOperations::copy (matrix[row], oldMatrix.matrix[row], dim);
    }

    ~Matrix()
    {
        // Deallocate memory...
        for (int i = 0; i < dim; ++i)
            delete[] matrix[i];
        
        delete[] matrix;
    }

    float** matrix;
    const int dim;
};

namespace MixingMatrixUtils
{
    /** Write matrix to log (useful for debugging) */
    static void logMatrix (Matrix& matrix)
    {
        std::stringstream ss;
        for (int row = 0; row < matrix.dim; ++row)
        {
            for (int col = 0; col < matrix.dim; ++col)
            {
                if (matrix.matrix[row][col] >= 0.0f)
                    ss << '+';

                ss << std::fixed << std::setprecision (4) << matrix.matrix[row][col];
                
                if (col < matrix.dim - 1)
                    ss << ", ";
            }
            ss << '\n';
        }
        
        String matString = String (ss.str());
        Logger::writeToLog (matString);

    }

    // Create identity matrix
    static void identityMatrix (Matrix& matrix)
    {
        for (int row = 0; row < matrix.dim; ++row)
            for (int col = 0; col < matrix.dim; ++col)
                matrix.matrix[row][col] = row == col ? 1.0f : 0.0f;
    }

    /** Orthonormal matrix */
    static void orthonormal (Matrix& matrix)
    {
        Random rand;
        float dcy = 2.0f;

        // set up A, a random diagonnaly symmetric matrix
        Matrix A (matrix.dim);
        for (int ind1 = 0; ind1 < matrix.dim; ++ind1)
        {
            for (int ind2 = ind1; ind2 < matrix.dim; ++ind2)
            {
                A.matrix[ind1][ind2] = (-1.0f + 2.0f * rand.nextFloat()) * pow (float (ind2 - ind1), dcy);
                A.matrix[ind2][ind1] = -A.matrix[ind1][ind2];
            }
        }

        // exponentiate matrix
        auto r8InMat = std::make_unique<double[]> (A.dim * A.dim);
        for (int row = 0; row < matrix.dim; ++row)
            for (int col = 0; col < matrix.dim; ++col)
                r8InMat[row * A.dim + col] = A.matrix[row][col];

        std::unique_ptr<double[]> r8OutMat;
        r8OutMat.reset (r8mat_expm1 (A.dim, r8InMat.get()));

        for (int row = 0; row < matrix.dim; ++row)
            for (int col = 0; col < matrix.dim; ++col)
                matrix.matrix[row][col] = (float) r8OutMat[row * A.dim + col];
    }

    /** Matrix generated for Music 424 HW7 */
    static void myMatrix (Matrix& matrix)
    {
        float myMatrix[12][12] = {
            {+0.63912f, +0.52380f, -0.41697f, +0.11043f, -0.12455f, -0.00443f, -0.00515f, -0.09814f, +0.00020f, -0.03819f, +0.25988f, +0.19213f, },
            {+0.53947f, +0.07753f, +0.46527f, -0.17110f, +0.14097f, -0.01164f, +0.03594f, -0.08002f, -0.03511f, -0.05676f, -0.16863f, -0.62978f, },
            {-0.38461f, +0.47142f, -0.03015f, +0.18607f, -0.21664f, -0.11385f, -0.10051f, -0.00205f, -0.13452f, +0.48722f, +0.18851f, -0.48300f, },
            {+0.11549f, -0.16923f, +0.24248f, +0.87265f, -0.00799f, +0.03538f, +0.03743f, -0.32297f, +0.10286f, +0.08233f, -0.09029f, +0.07191f, },
            {-0.10333f, +0.15643f, -0.20733f, +0.08803f, +0.75408f, +0.05174f, +0.56302f, +0.04113f, +0.06349f, +0.11754f, +0.02935f, -0.07359f, },
            {-0.00070f, -0.02606f, -0.04298f, +0.00534f, -0.19633f, +0.94148f, +0.10985f, +0.12521f, +0.13780f, +0.05288f, +0.06379f, -0.13754f, },
            {+0.00604f, -0.02103f, +0.14505f, -0.06344f, -0.52588f, -0.17968f, +0.80939f, +0.01936f, -0.07884f, -0.01637f, +0.00890f, +0.05980f, },
            {+0.09295f, +0.06716f, -0.03362f, +0.33674f, -0.04308f, -0.11748f, -0.03299f, +0.88120f, +0.03701f, -0.15751f, -0.21566f, -0.07987f, },
            {+0.00553f, +0.03571f, +0.11946f, -0.11200f, -0.05422f, -0.16669f, -0.00966f, +0.04689f, +0.94259f, +0.15459f, +0.16273f, +0.00220f, },
            {+0.03050f, +0.00691f, -0.44899f, -0.06089f, -0.14023f, -0.02089f, +0.02007f, -0.18515f, +0.15422f, +0.20397f, -0.81415f, -0.10350f, },
            {-0.29988f, +0.19520f, -0.17512f, +0.12790f, -0.07240f, -0.01997f, +0.02620f, -0.21981f, +0.16471f, -0.79869f, +0.01336f, -0.32416f, },
            {-0.17027f, +0.63184f, +0.48624f, -0.06766f, +0.06469f, +0.14788f, -0.02731f, -0.02455f, +0.00962f, -0.07780f, -0.34542f, +0.42181f, },
           };

        for (int row = 0; row < matrix.dim; ++row)
        {
            for (int col = 0; col < matrix.dim; ++col)
                matrix.matrix[row][col] = myMatrix[row][col];
        }
    }
}

#endif // MIXING_MATRIX_UTILS_H_INCLUDED
