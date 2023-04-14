#ifndef ASYNC_H
#define ASYNC_H
#include <cstddef>

namespace async {

int connect(const std::size_t bulk_size);
void receive(const char* data, const std::size_t length, const int context);
void disconnect(const int context);

}
#endif // ASYNC_H
