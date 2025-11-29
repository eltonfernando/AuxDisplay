#pragma once
#include <cstdint>
#include <vector>

class Image {
   public:
    Image(int w, int h, int channels, std::vector<uint8_t> data) noexcept;

    int getWidth() const noexcept;
    int getHeight() const noexcept;
    int getChannels() const noexcept;

    const uint8_t* data() const noexcept;
    const std::vector<uint8_t>& raw() const noexcept;

    Image crop(int x, int y, int w, int h) const;  // necessário para display

   private:
    int width_;
    int height_;
    int channels_;
    std::vector<uint8_t> data_;
};

enum class Endianness { Little, Big };

std::vector<uint8_t> image_to_RGB565(const Image& img, Endianness e);
