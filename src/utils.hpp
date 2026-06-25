#pragma once
#include "pch.h"

namespace utils
{
    typedef std::complex<double> Complex;
    typedef std::valarray<Complex> ComplexArray;

    void CalcFFT(ComplexArray& x)
    {
        const size_t N = x.size();
        if (N <= 1) return;

        // divisao e
        ComplexArray even = x[std::slice(0, N/2, 2)];
        ComplexArray odd = x[std::slice(1, N/2, 2)];

        // calc
        CalcFFT(even);
        CalcFFT(odd);

        // combina os dois
        for (size_t k = 0; k < N / 2; ++k)
        {
            Complex t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
            x[k] = even[k] + t;
            x[k + N / 2] = even[k] - t;
        }
    }

    ComplexArray ParseDatFile(const char* const filePath)
    {
        std::ifstream iFs(filePath);
        if(iFs.fail() ||! iFs.is_open())
            return {};
        
        std::vector<Complex> values;
        
        std::string line;
        while(std::getline(iFs, line))
        {
            if(line.empty())
                continue;
            
            std::stringstream ssLine(line);
            
            double point;
            while(!ssLine.eof())
                ssLine >> point;
            
            values.push_back(point);
        }

        return ComplexArray(values.data(), values.size());
    }
}