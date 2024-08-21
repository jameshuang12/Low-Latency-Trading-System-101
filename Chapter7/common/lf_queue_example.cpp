#include "thread_utils.h"
#include "lf_queue.h"

struct MyStruct {
    int d_[3];
};

using namespace Common;

auto consumeFunction(LFQueue<MyStruct>* lfq) {
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(5s);

    while(lfq->size()) {
        const auto d = lfq->getNextToRead();
        lfq->updateReadIndex();


        std::cout << "consumeFunction read elem:" << d->d_[0] << "," << d->d_[1] << ","
        << d->d_[2] << " lfq-size:" << lfq->size() << std::endl;
        std::this_thread::sleep_for(1s);
    }

    std::cout << "consumeFunction exiting" << std::endl;
}

int main(int, char **) {
    ;
}