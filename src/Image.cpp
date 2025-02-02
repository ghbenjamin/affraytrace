#include <Image.h>

#include <ranges>
#include <algorithm>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


namespace affraytrace
{

Image::Image( int const width, int const height )
    : m_width(width), m_height(height), m_rowBytes(width * STRIDE), m_data(height*m_rowBytes, 0.0f)
{}

void Image::save(std::string const &filename)
{
    // Map from [0,1] float RGB space to [0,255] int RGB space
    auto const scaleF = []( const float f ) -> std::uint8_t {
        return std::floor(255.999f * f);
    };

     std::vector<uint8_t> scaledData = m_data
        | std::views::transform(scaleF)
        | std::ranges::to<std::vector>();

    stbi_write_png(filename.c_str(), m_width, m_height, Image::STRIDE, scaledData.data(), m_rowBytes );
}

void Image::setPixel(const int x, const int y, Colour const &colour)
{
    m_data[x * STRIDE + y * m_rowBytes] = colour.x();
    m_data[x * STRIDE + y * m_rowBytes + 1] = colour.y();
    m_data[x * STRIDE + y * m_rowBytes + 2] = colour.z();
}

}
