#include "Image.hpp"
#include <stdexcept>

Image::Image(int w, int h, int channels, std::vector<uint8_t> data) noexcept
    : width_(w), height_(h), channels_(channels), data_(std::move(data)) {}

int Image::getWidth() const noexcept {
    return width_;
}
int Image::getHeight() const noexcept {
    return height_;
}
int Image::getChannels() const noexcept {
    return channels_;
}

const uint8_t* Image::data() const noexcept {
    return data_.data();
}
const std::vector<uint8_t>& Image::raw() const noexcept {
    return data_;
}

Image Image::crop(int x, int y, int w, int h) const {
    if (w <= 0 || h <= 0) throw std::runtime_error("crop: dimensões inválidas");

    std::vector<uint8_t> out;
    out.reserve(w * h * channels_);

    for (int row = 0; row < h; ++row) {
        int srcY              = y + row;
        const uint8_t* srcPtr = data_.data() + (srcY * width_ + x) * channels_;
        out.insert(out.end(), srcPtr, srcPtr + w * channels_);
    }

    return Image(w, h, channels_, std::move(out));
}

std::vector<uint8_t> image_to_RGB565(const Image& img, Endianness e) {
    const int w  = img.getWidth();
    const int h  = img.getHeight();
    const int ch = img.getChannels();

    if (ch != 3 && ch != 4) throw std::invalid_argument("Image must be RGB or RGBA");

    const size_t pixels = static_cast<size_t>(w) * h;
    const uint8_t* src  = img.data();

    std::vector<uint8_t> out;
    out.reserve(pixels * 2);

    for (size_t i = 0; i < pixels; ++i) {
        const uint8_t r8 = src[i * ch + 0];
        const uint8_t g8 = src[i * ch + 1];
        const uint8_t b8 = src[i * ch + 2];

        uint16_t r5 = r8 >> 3;
        uint16_t g6 = g8 >> 2;
        uint16_t b5 = b8 >> 3;

        uint16_t rgb565 = (r5 << 11) | (g6 << 5) | b5;

        if (e == Endianness::Big) {
            out.push_back(rgb565 >> 8);
            out.push_back(rgb565 & 0xFF);
        } else {
            out.push_back(rgb565 & 0xFF);
            out.push_back(rgb565 >> 8);
        }
    }

    return out;
}
