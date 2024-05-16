//
// Created by zhaojieyi on 2024/5/14.
//
#include <algorithm>
#include "example_1.h"
#include "example_2.h"
#include "example_3.h"

int main() {
    //    {
    //        using namespace example_1;
    //        Coroutine();  // basic
    //    }
    //    {
    //        using namespace example_2;
    //        DoGenerate();     //  Generator
    //        DoFibonacci(10);  //  Fibonacci
    //    }

    {
        using namespace example_3;
        //        Generator<std::string> g_str = Fibonacci().Map<std::string>([](int i) { return std::to_string(i); });
        //        Generator<std::string> g_str_2 = Fibonacci().Map2([](int i) { return std::to_string(i); });
        //
        //        Generator<int>::From(1, 2, 3, 4)
        //            .FlatMap([](auto i) -> Generator<int> {
        //                for (int j = 0; j < i; j++) {
        //                    co_yield j;
        //                }
        //            })
        //            .ForEach([](auto i) {
        //                if (0 == i) {
        //                    std::cout << std::endl;
        //                }
        //                std::cout << "*";
        //            });
        //
        //        auto result = Generator<int>::From(1, 2, 3, 4, 5, 6).fold(1, [](auto acc, auto i) {
        //            return acc * i;  // 计算阶乘
        //        });
        //        std::cout << result << std::endl;

        Generator<int>::From(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
            .Filter([](auto i) {
                std::cout << "Filter: " << i << std::endl;
                return i % 2 == 0;
            })
            .Map2([](auto i) {
                std::cout << "map: " << i << std::endl;
                return i * 3;
            })
            .FlatMap([](auto i) -> Generator<int> {
                std::cout << "flat_map: " << i << std::endl;
                for (int j = 0; j < i; ++j) {
                    co_yield j;
                }
            })
            .Take(10)
            .ForEach([](auto i) { std::cout << "for_each: " << i << std::endl; });
    }

    return 0;
}