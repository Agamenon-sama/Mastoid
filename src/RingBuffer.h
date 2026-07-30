#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity);

    void write(const float *data, size_t size);
    void read(float *dest, size_t size);
    bool hasEnoughSamples(size_t n) const;

private:
    std::vector<float> _buffer;
    size_t _capacity;
    size_t _writePosition = 0;
    size_t _totalWritten = 0;
};

#endif // RINGBUFFER_H
