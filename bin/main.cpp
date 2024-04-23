#include "LA.h"
#include "SA.h"
#include <thread>
#include "Logger.h"

double sum = 0;
std::vector<Token> Token::tokens = std::vector<Token>();
int Token::state = Wait;
std::mutex Token::mutex = std::mutex();

void test() {
    Logger::log("Test started.");
    Token::tokens.clear();
    Token::state = Wait;

    auto st_time = std::chrono::high_resolution_clock::now();

    LA la = LA();
    SA sa = SA();

//    la();
//    sa();
    std::thread la_th = std::thread( la );
    std::thread sa_th = std::thread( sa );
    la_th.join();
    sa_th.join();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> tim = end_time - st_time;
    Logger::log("Total time: " + std::to_string(tim.count()));
    sum += tim.count();

    Generation::inst().poliz.print_self();
    Generation::inst().print_got();
    std::cout << Generation::inst().poliz.calc(Generation::inst().got["main"]) << std::endl;

}

int main() {
    int N = 1;
    std::cout << "N: " << N << std::endl;
    auto st_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        test();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> tim = end_time - st_time;
    Logger::log("Testing time: " + std::to_string(tim.count()));
    sum /= N;
    Logger::log("Average: " + std::to_string(sum));
	return 0;
}


