#include "rgbimage.h"
#include "color.h"
#include "assert.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include "FreeImage.h"

RGBImage::RGBImage(unsigned int Width, unsigned int Height) {
    m_Height = Height;
    m_Width = Width;
    m_Image = new Color[m_Width * m_Height];
}

RGBImage::~RGBImage() {
    delete this->m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c) {
    m_Image[m_Width * y + x] = c;

}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const {
    return m_Image[m_Width * y + x];
}

unsigned int RGBImage::width() const {
    return m_Width;
}

unsigned int RGBImage::height() const {
    return m_Height;
}

// //modifizirte texture load funktion basierend auf Texture.cpp. laden von texturen ohne 
// erstellung von Texture objekten, Vor allem zum laden von sprites in imgui
bool RGBImage::loadFromDisk(const char* Filename,  unsigned int& width,  unsigned int& height,  unsigned char*& finalData)
{
    
        FREE_IMAGE_FORMAT ImageFormat = FreeImage_GetFileType(Filename, 0);
        if (ImageFormat == FIF_UNKNOWN)
            ImageFormat = FreeImage_GetFIFFromFilename(Filename);

        if (ImageFormat == FIF_UNKNOWN)
        {
            //        std::cout << "Warning: Unkown texture format: " << Filename << std::endl;
            return false;
        }

        FIBITMAP* pBitmap = FreeImage_Load(ImageFormat, Filename);

        if (pBitmap == NULL)
        {
            //     std::cout << "Warning: Unable to open texture image " << Filename << std::endl;
            return false;
        }

        FREE_IMAGE_TYPE Type = FreeImage_GetImageType(pBitmap);
        assert(Type == FIT_BITMAP);

        unsigned int Width = FreeImage_GetWidth(pBitmap);
        unsigned int Height = FreeImage_GetHeight(pBitmap);
        unsigned int bpp = FreeImage_GetBPP(pBitmap);
        assert(bpp == 32 || bpp == 16 || bpp == 24);

        unsigned char* data = new unsigned char[Width * Height * 4];
        unsigned char* dataPtr = data - 1;

        if (data == NULL)
        {
            FreeImage_Unload(pBitmap);
            return false;
        }


        RGBQUAD c;
        for (unsigned int i = 0; i < Height; ++i)
            for (unsigned int j = 0; j < Width; ++j)
            {
                FreeImage_GetPixelColor(pBitmap, j, Height - i - 1, &c);
                *(++dataPtr) = c.rgbRed;
                *(++dataPtr) = c.rgbGreen;
                *(++dataPtr) = c.rgbBlue;
                if (bpp == 32)
                    *(++dataPtr) = c.rgbReserved;
                else
                    *(++dataPtr) = 255;
            }

        FreeImage_Unload(pBitmap);
        width = Width;
        height = Height;
        finalData = data;

        //delete[] data;
        return true;
    
}

unsigned char RGBImage::convertColorChannel(float v) {
    float f2 = std::fmax(0.0, std::fmin(1.0, v));
    return floor(f2 == 1.0 ? 255 : f2 * 256.0);
    //return v*255;

}

//function to generate a 24 bit bmp file given the Filename and an Array with the pixel values
//bool RGBImage::saveToDisk( const char* Filename)
//{
//    std::ofstream f;
//    f.open(Filename, std::ios::out | std::ios::binary);
//    if (!f.is_open()){
//        std::cout << "file cant be opened";
//        return false;
//    }
//    unsigned char bmpPad[3] = {0,0,0};
//    const int paddingAmount = ((4-(m_Width*3)%4)%4);
//
//    const int fileHeaderSize=14;
//    const int informationHeaderSize=40;
//    const int fileSize = fileHeaderSize + informationHeaderSize +m_Width*m_Height * 3 +paddingAmount*m_Height;
//
//    unsigned char fileHeader[fileHeaderSize];
//    unsigned char informationHeader[informationHeaderSize];
//
//    fileHeader[0] = 'B';
//    fileHeader[1] = 'M';
//    fileHeader[2] = fileSize;
//    fileHeader[3] = fileSize >> 8;
//    fileHeader[4] = fileSize >> 16;
//    fileHeader[5] = fileSize >> 24;
//    fileHeader[10] = fileHeaderSize + informationHeaderSize;
//
//    informationHeader[0] = informationHeaderSize;
//    informationHeader[4] = m_Width;
//    informationHeader[5] = m_Width >> 8;
//    informationHeader[6] = m_Width >> 16;
//    informationHeader[7] = m_Width >> 24;
//    informationHeader[8] = m_Height;
//    informationHeader[9] = m_Height >> 8;
//    informationHeader[10] = m_Height >> 16;
//    informationHeader[11] = m_Height >> 24;
//    informationHeader[12] = 1;
//    informationHeader[14] = 3;
//
//    f.write((char*)fileHeader, fileHeaderSize);
//    f.write((char*)informationHeader, informationHeaderSize);
//
//    for(int y = 0; y < m_Height; y++)
//    {
//        for(int x = 0; x < m_Width; x++)
//        {
//            Color col = getPixelColor(x,y);
//
//
//            unsigned char r = convertColorChannel(col.R);
//            unsigned char g = convertColorChannel(col.G);
//            unsigned char b = convertColorChannel(col.B);
//            f.write((char*)&b, sizeof(b));
//            f.write((char*)&g, sizeof(g));
//            f.write((char*)&r, sizeof(r));
////unsigned char color_converted[] = {b,g,r};
////            f.write(reinterpret_cast<char*>(color_converted),3);
//        }
//        f.write((char*)bmpPad, paddingAmount);
//    }
//    f.close();
//    return true;
//}
bool RGBImage::saveToDisk(const char* Filename) {
    // �ffnen der Datei zum Schreiben im bin�ren Modus
    FILE* file = fopen(Filename, "wb");
    if (file == nullptr) {
        std::cerr << "Fehler beim �ffnen der Datei zum Schreiben." << std::endl;
        return false;
    }

    // BMP-Dateiheader schreiben
    unsigned int fileSize = 54 + 3 * m_Width * m_Height;
    unsigned char fileHeader[14] = {
            'B', 'M',             // Dateityp (Bitmap)
            fileSize, fileSize >> 8, fileSize >> 16, fileSize >> 24, // Dateigr��e
            0, 0, 0, 0,           // Reserviert
            54, 0, 0, 0           // Offset zum Bild-Datenbereich
    };

    unsigned char infoHeader[40] = {
            40, 0, 0, 0,          // Info-Headergr��e
            m_Width, m_Width >> 8, m_Width >> 16, m_Width >> 24, // Bildbreite
            -m_Height, -m_Height >> 8, -m_Height >> 16, -m_Height >> 24, // Bildh�he
            1, 0,                // Farbebenen (immer 1)
            24, 0,               // Bittiefe (24-Bit RGB)
            0, 0, 0, 0,          // Keine Kompression
            0, 0, 0, 0,          // Bildgr��e (wird sp�ter berechnet)
            0, 0, 0, 0,          // X-Aufl�sung
            0, 0, 0, 0           // Y-Aufl�sung
    };

    // Bildgr��e im Header aktualisieren
    infoHeader[20] = 3 * m_Width;
    infoHeader[21] = 3 * m_Width >> 8;
    infoHeader[22] = 3 * m_Width >> 16;
    infoHeader[23] = 3 * m_Width >> 24;

    // Dateiheader und Info-Header in die Datei schreiben
    fwrite(fileHeader, 1, 14, file);
    fwrite(infoHeader, 1, 40, file);

    // Bildpixel in die Datei schreiben
    for (unsigned int y = 0; y < m_Height; y++) {
        for (unsigned int x = 0; x < m_Width; x++) {
            const Color& pixelColor = getPixelColor(x, y);
            unsigned char r = convertColorChannel(pixelColor.R);
            unsigned char g = convertColorChannel(pixelColor.G);
            unsigned char b = convertColorChannel(pixelColor.B);
            fputc(b, file);
            fputc(g, file);
            fputc(r, file);
        }
    }

    // Datei schlie�en
    fclose(file);
    return true;
}


