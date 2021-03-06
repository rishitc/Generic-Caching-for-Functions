#include <math.h>
#include <limits.h>
#include <cstdlib>
#include <string>
#include <random>
#include <memory>

int prime_factors(long long int n) 
{
    if (n <= 1)
        return 0;
    int result = 0;
    while (n % 2 == 0) 
    { 
        ++result;
        n = n/2; 
    } 
    for (int i = 3; i <= sqrt(n); i = i + 2) 
    { 
        while (n % i == 0) 
        { 
            ++result;
            n = n/i; 
        } 
    } 
    if (n > 2) 
        ++result;
    return result;
}

long long int test_case_compute(long long int val)
{
    val <<= 30;
    val += 1e8;

    return prime_factors(val);
}

struct header{
    public:
        std::string url;
        std::string content_type;
        std::string content;
        header() : url{}, content_type{}, content{} {}
};

std::string random_string(std::size_t length)
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
    std::string random_string;
    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }
    return random_string;
}


header http_header_gen(int rand_value)
{
    header header_n;
    header_n.url = random_string(200);
    header_n.content_type = random_string(100);
    header_n.content = random_string(10240);
    return header_n;
}


std::shared_ptr<header> http_header_gen_ptr(int rand_value)
{
    std::shared_ptr<header> header_n = std::make_shared<header> ();
    header_n -> url = random_string(200);
    header_n -> content_type = random_string(100);
    header_n -> content = random_string(10240);
    return header_n;
}


template<typename func>
void test_case_gen(func& _func)
{
    srand(1);
    for(unsigned int i = 0; i < 10000; ++i)
    {
        unsigned int v = rand()%1000 + 100000;
        _func(v);
    }
}

