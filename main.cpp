#include <iostream>
#include <cmath>
#include <tgmath.h>
#include <vector>
#include <map>
#include <fstream>

#define CONSTANCE_E 2.7182818284590
#define PI          3.1415926535

const char * raw = R"(
p 10 10 # �����ϻ滭���ڴ�С800 600 4:3����Ȼ�˶��켣��ʾΪ��Բ
s 1 # �����ٶ�
vl 4096 # �������������#�ź�Ҫ���Ͽո���򱨴�
fl 6000 # ��������
ss 3 # �����ٶ�
fenv 5 # �洢����,Ŀǰû��
mp 1 # �ƶ��ٶ�

# v x[����x����] y[����y����] r[������ʼ��ת��] t[������ת����,������תһȦ]  ˳��ɽ���

)";

using namespace std;

struct Complex{
    float real;
    float imag;
    Complex(float r = 0,float i = 0){
        real = r;
        imag = i;
    }
    Complex(const Complex& c){
        real = c.real;
        imag = c.imag;
    }
    float length(){
        return sqrt(imag * imag + real * real);
    }
};


Complex apow(Complex c,float basen = CONSTANCE_E){
    float base = pow(basen,c.real);
    Complex cp;
    cp.real = base * pow(cos(c.imag),(basen == CONSTANCE_E)?1:log(basen));
    cp.imag = base * pow(sin(c.imag),(basen == CONSTANCE_E)?1:log(basen));
    return cp;
}
Complex epow(Complex c){
    float base = pow(CONSTANCE_E,c.real);
    Complex cp;
    cp.real = base * cos(c.imag);
    cp.imag = base * sin(c.imag);
    return cp;
}

ostream& operator << (ostream&os,Complex& c){
    os <<"(" <<c.real << "," << c.imag << ")";
    return os;
}

istream& operator >> (istream&os,Complex& c){
    os >> c.real >> c.imag;
    return os;
}

Complex operator*(Complex a,Complex b){
    return Complex(a.real * b.real - a.imag * b.imag,a.real * b.imag + a.imag * b.real);
}

int main()
{
    vector<Complex> points;
    map<int,Complex> args;
    unsigned int counts;
    int freqbeg,freqend;
    float etaT;

    ifstream ifs("1.in");
    ofstream ofs("1.out");
    ofstream fft("1.math");

    fft << raw;

    ifs >> counts;
    ifs >> freqbeg >> freqend;
    ifs >> etaT;
    for(unsigned int i = 0;i < counts;++i){
        Complex c;
        ifs >> c;
        points.push_back(c);
    }

    ///Processing
    for(int i = freqbeg;i <= freqend;++i){
        ofs << "\n";
        ofs << "Frequency:" << i << "\n";
        if(i%800 == 0)cout << "Current:" << i << "\n";
        ///Summation
        Complex sum;
        for(unsigned int t = 0;t < points.size();++t){
            float pt = (float)t / points.size();
            Complex frem = epow(Complex(0,2 * i * PI * pt * etaT));
            ofs << frem << " " << frem.length() << " ";
            Complex result = frem * points[t];
            ofs << result.length() << "\n";
            sum.real += result.real;
            sum.imag += result.imag;
        }
        sum.real /= (float)points.size();
        sum.imag /= (float)points.size();

        if(abs(sum.real) <= 1e-6)sum.real = 0;
        if(abs(sum.imag) <= 1e-6)sum.imag = 0;

        ofs << "Summation:" << sum << "\n";
        float rot = (i==0)?(1145141919):(1.0/i);
        fft << "v x" << sum.real << " y" << sum.imag << " r0 t" << rot << "\n";


    }
    ifs.close();
    ofs.close();
    fft.close();

    return 0;
}
