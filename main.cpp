#include <iostream>
#include <cmath>
#include <tgmath.h>
#include <vector>
#include <map>
#include <fstream>

#define CONSTANCE_E 2.7182818284590
#define PI          3.1415926535

const char * raw = R"(
p 10 10 # 连接上绘画窗口大小800 600 4:3，不然运动轨迹显示为椭圆
s 1 # 计算速度
vl 4096 # 顶点最多数量，#号后要加上空格否则报错
fl 6000 # 放弃休眠
ss 3 # 缩放速度
fenv 5 # 存储精度,目前没用
mp 1 # 移动速度

# v x[向量x坐标] y[向量y坐标] r[向量初始旋转角] t[向量旋转周期,多少秒转一圈]  顺序可交换

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
