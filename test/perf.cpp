#include "redispp.h"
#include <sys/time.h>
#include <iostream>
#include <boost/bind.hpp>
#include <list>

using namespace redispp;

void runFunc(size_t count)
{
    Connection conn("127.0.0.1", "6379", "password", false);

    std::string key = "somemediumkey2";
    std::string value = "somemediumvalue";

    const size_t chunkFactor = 256;
    VoidReply replies[chunkFactor];

    timeval begin;
    gettimeofday(&begin, NULL);

    for(size_t i = 0; i < count; ++i)
    {
        const size_t index = i & (chunkFactor - 1);
        replies[index] = conn.set(key, value);
        if(index == (chunkFactor - 1))
        {
            for(size_t j = 0; j < chunkFactor; ++j)
            {
                replies[j].result();
            }
        }
    }

    timeval end;
    gettimeofday(&end, NULL);

    const long long beginUsec = (long long)begin.tv_sec * 1000000LL + begin.tv_usec;
    const long long endUsec = (long long)end.tv_sec * 1000000LL + end.tv_usec;
    std::cout << count << " writes in " << (double)(endUsec - beginUsec)/1000000.0 << " ~= " << (double)count * 1000000.0/(double)(endUsec - beginUsec) << std::endl;
}

int main(int argc, char* argv[])
{
    int count = argc > 1 ? atoi(argv[1]) : 100000;
    runFunc(count);
    return 1;
}