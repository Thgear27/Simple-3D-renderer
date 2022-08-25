#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cstring> // Para utilizar std::memcpy()
#include <fstream> // Para utilizar std::ifstream y std::ofstream

#pragma pack(push, 1) // Le dice al compilador que guarde los datos para un maximo de 1 byte

// std::int8_t ocupa un byte igual que char
// El header es la parte inicial del archivo .tga y ocupa 18 bytes, ver wikipedia
struct TGA_Header {
    std::int8_t idlength;
    std::int8_t colormaptype;
    std::int8_t datatypecode;
    short colormaporigin;
    short colormaplength;
    std::int8_t colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    std::int8_t bitsperpixel;
    std::int8_t imagedescriptor;
};
#pragma pack(pop) // El anterior pack(push, 1) se anula y vuelva a almacenar para un máximo de 4 bytes
                  // almenos para mi computadora(depende de la arquitectura del cpu)
// Estructura donde se guarda la informacion del color, ocupa 8 bytes, pero al momento de escribir
// informacion en el archivo de salidad se leen los 4 primeros bits normalmente
struct TGAColor {
    // Union anonima, se puede acceder a sus miembros como si sus elementos fueran parte del contenedor
    union {
        struct {
            std::uint8_t b, g, r, a; // Se ordenan al revez debido a que los bytes son ornados con little endian
        };
        std::uint8_t raw[4];
        unsigned int val;
    };
    int bytespp; // Bytes por pixel

    // Constructor por defecto

    TGAColor() : val(0), bytespp(1) {}

    // Otros contructores utiles

    TGAColor(std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A) : b(B), g(G), r(R), a(A), bytespp(4) {}
    TGAColor(int v, int bpp) : val(v), bytespp(bpp) {}
    TGAColor(const TGAColor& c) : val(c.val), bytespp(c.bytespp) {}
    TGAColor(const std::uint8_t* p, int bpp) : val(0), bytespp(bpp) { std::memcpy(raw, p, bpp); }

    // Redefinicion del operador de asigancion
    TGAColor& operator=(const TGAColor& c) {
        if (this != &c) { // Verifica que c no tenga la misma direccion de memoria que this, que no sean el mismo objeto
            bytespp = c.bytespp;
            val = c.val;
        }
        // retorna la dereferenciacion de this y actua como una referencia
        return *this;
    }
};

// Clase que engloba representa una imagen .tga, capaz de generar un archivo de salidad .tga
class TGAImage {
protected:                 // Los elementos pueden ser accedidos por miembros de TGAImage, friends y clases hijas
    unsigned char* m_data; // Representa los datos de la imagen(pixeles)
    int m_width;           // Ancho en pixeles
    int m_height;          // Altura en pixeles
    int m_bytespp;         // Bytes por pixel

    bool load_rle_data(std::ifstream& in);    // Lee los datos del header y los guarda en la referencia in
    bool unload_rle_data(std::ofstream& out); // Vuelca los datos de la imagen .tga en el la refernecia out

public:
    enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4 }; // Representan los bits por pixel

    // Constructurores

    // Constructor por defecto: Inicializa todo con 0 y data con nullptr
    TGAImage();

    // Inicializa data con un arreglo de tamanio w * t * bpp
    TGAImage(int w, int h, int bpp);

    // Constructor de
    TGAImage(const TGAImage& img);

    // Reasinacion del operador de asignacion
    TGAImage& operator=(const TGAImage& img);
    // De aqui en adelante los metodos hacen los que dice su nombre literalmente

    bool read_tga_file(const char* filename);
    bool write_tga_file(const char* filename, bool rle = true);
    bool flip_horizontally();
    bool flip_vertically();
    bool scale(int w, int h); // !Sinceramente no se lo que hace
    TGAColor get(int x, int y);
    bool set(int x, int y, const TGAColor& c);
    ~TGAImage();
    int get_width();
    int get_height();
    int get_bytespp();

    // Retornar data
    unsigned char* buffer();
    void clear();
};

#endif //__IMAGE_H__
