#include "RingBuffer.h"

#include <QtAssert>

RingBuffer::RingBuffer(size_t capacity) : _buffer(capacity, 0.f), _capacity(capacity) {}

void RingBuffer::write(const float *data, size_t size) {
    Q_ASSERT(data && "Writing to invalid ptr");

    for (size_t i = 0; i < size; i++) {
        if (_writePosition >= _capacity) _writePosition = 0;

        _buffer[_writePosition] = data[i];
        _writePosition++;
    }

    _totalWritten += size;
}

void RingBuffer::read(float *dest, size_t size) {
    Q_ASSERT(_capacity >= size);

    size_t start = (_writePosition - size + _capacity) % _capacity;
    for (size_t i = 0; i < size; i++) {
        dest[i] = _buffer[start];

        start = (start + 1 == _capacity) ? 0 : start + 1;
    }
}

bool RingBuffer::hasEnoughSamples(size_t n) const {
    return _totalWritten >= n;
}
