//MIT License
//
//Copyright(c) 2021 m1bcodes
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this softwareand associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright noticeand this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//
//fork me on GitHub: https://github.com/m1bcodes/IIR_Ringbuffer_Filter


// IIR_Ringbuffer_Filter.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
// based on https://stackoverflow.com/questions/22538687/implementation-of-iir-lowpass-filter-in-c


#include <iostream>
#include <vector>
#include <fstream>

class IIRCircularBufferFilter
{
public:
    IIRCircularBufferFilter(const std::vector<double>& coef_b, const std::vector<double>& coef_a)
        : a(coef_a)
        , b(coef_b)
    {
        len = std::max(b.size(), a.size());
        x.resize(len);
        y.resize(len);
        clear();
    }

    ~IIRCircularBufferFilter()
    {

    }

    void clear()
    {
        idx = 0;
        for (size_t i = 0; i < x.size(); ++i)
        {
            x[i] = 0;
            y[i] = 0;
        }
    }
    
    /*
    m=max(length(a),length(b));
    yr = zeros(size(t));
    n=0;
    x=zeros(1,m);
    y=zeros(1,m);
    for i1=1:length(t)
        x(1+n)=step(i1);
        s = b(1+0) * x(1+n);
        for i2 = 2:length(b)
            s=s + b(1+i2-1) * x(1+mod(n-i2+1+m,m))
        end
        for i2=2:length(a)
            s = s - a(1+i2-1) * y(1+mod(n-i2+1+m,m));
        end
        y(1+n)=s;
        n=mod(n+1,m);
        yr(i1)=y(1+n);
    end
    */
    double push(double v)
    {
        x[idx] = v;
        double s = b[0] * x[idx];
        for (size_t i = 1; i < b.size(); i++) {
            s += b[i] * x[(idx - i + len) % len];
        }
        for (size_t i = 1; i < a.size(); i++) {
            s -= a[i] * y[(idx - i + len) % len];
        }

        y[idx] = s;
        idx = (idx + 1) % len;
        return y[idx];
    }

private:
    std::vector<double> a, b, x, y;
    int idx, len;
};

const std::vector<double> a{ 1.0, -0.577240524806303, 0.421787048689562, -0.056297236491843 };
const std::vector<double> b{ 0.098531160923927, 0.295593482771781, 0.295593482771781, 0.098531160923927 };

int main()
{
    std::cout << "Hello World!\n";
    IIRCircularBufferFilter f(b, a);
    std::ofstream fs("out.txt");
    for (int i = 0; i < 100; i++)
    {
        double y = 0.2;
        if (i > 50) y = 1;
        double y1 = f.push(y);
        fs << i << "; " << y << "; " << y1 << "\n";
    }
}
