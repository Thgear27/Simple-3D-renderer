#include "tgaimage.h"
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>  // Para usar std::ifstream y std::ofstream
#include <iostream> // std::cout

TGAImage::TGAImage() : m_data(nullptr), m_width(0), m_height(0), m_bytespp(0) {}

TGAImage::TGAImage(int w, int h, int bpp) : m_data(nullptr), m_width(w), m_height(h), m_bytespp(bpp) {
    unsigned long nbytes = m_width * m_height * m_bytespp; // nbytes representa la cantidad de bytes que necesita m_data
    m_data = new unsigned char[nbytes];                    // Cada pixel requiere m_bytespp por pixel
    std::memset(m_data, 0, nbytes);                        // Se le asigna 0 a todos los bytes de data
}

TGAImage::TGAImage(const TGAImage& img) {
    m_width = img.m_width;
    m_height = img.m_height;
    m_bytespp = img.m_bytespp;
    unsigned long nbytes = m_width * m_height * m_bytespp;
    m_data = new unsigned char[nbytes];
    std::memcpy(m_data, img.m_data, nbytes); // std::mempcpy copia los bytes de img.m_data a m_data
}

TGAImage::~TGAImage() {
    if (m_data)          // Verifica que m_data no sea un puntero nulo
        delete[] m_data; // Libera la memoria assignada y el SO vuelve a tener esa memoria a su disposicion
}

TGAImage& TGAImage::operator=(const TGAImage& img) {
    if (this != &img) { // Verifica: Direccion de memoria de img sea diferente que this(ptr al objeto siendo asignado)
        if (m_data)     // Verifica: m_data no sea  un puntero nulo
            delete[] m_data; // Se libera la memoria de m_data para luego copiarle la de img.m_data
        m_width = img.m_width;
        m_height = img.m_height;
        m_bytespp = img.m_bytespp;
        unsigned long nbytes = m_width * m_height * m_bytespp;
        m_data = new unsigned char[nbytes];
        std::memcpy(m_data, img.m_data, nbytes);
    }
    return *this;
}

bool TGAImage::read_tga_file(const char* filename) {
    // Como leer un .tga no necesriamente coincidira *this, entonces es como crear un nuevo objeto
    if (m_data)          // Verifica: m_data no sea un puntero nulo
        delete[] m_data; // Libera la memoria de m_data
    m_data = nullptr;
    std::ifstream in;                    // Objeto de tipo std::ifstream
    in.open(filename, std::ios::binary); // Abre el archivo en modo binario, no vamos a interpretar texto con formato
    if (!in.is_open()) {                 // Verifica que in este asociado a un archivo de verdad :v
        std::cerr << "can't open file " << filename << "\n";
        in.close();
        return false;
    }
    TGA_Header header; // objeto de tipo TGA_Header, pesa (18 bytes)

    in.read((char*)&header, sizeof(header)); // Lee 18 caracteres(1 bytes) del archivo asociado a in
    // Aunqe aqui se estan leyendo caracteres, en realidad se trataran como numero enteros de 1 bytes

    if (!in.good()) { // Retorna true si ninguna de las banderas de error han cambiado, badbit, failbit, eofbit
        in.close();
        std::cerr << "an error occured while reading the header\n";
        return false;
    }
    m_width = header.width; // Copia los valores del header a los valores del TGAImage
    m_height = header.height;
    m_bytespp = header.bitsperpixel / 8; // Un bytes son 8 bits

    // Condicional que verifica que la altura, el ancho y los valores de m_bytespp sean aceptables
    if (m_width <= 0 || m_height <= 0 || (m_bytespp != GRAYSCALE && m_bytespp != RGB && m_bytespp != RGBA)) {
        in.close();
        std::cerr << "bad bpp (or width/height) value\n";
        return false;
    }

    // Se asinga a m_data la cnatidad de bytes necesarios basandose en los datos de header
    unsigned long nbytes = m_bytespp * m_width * m_height;
    m_data = new unsigned char[nbytes];

    /*
        0 no image data is present
        1 uncompressed color-mapped image
        2 uncompressed true-color image                         (***)
        3 uncompressed black-and-white (grayscale) image        (***)
        9 run-length encoded color-mapped image
        10 run-length encoded true-color image                  (***)
        11 run-length encoded black-and-white (grayscale) image (***)
    */

    if (header.datatypecode == 3 || header.datatypecode == 2) { // True si no es rle
        in.read((char*)m_data, nbytes);                         // Copia los bytes del archivo asociado en m_data
        if (!in.good()) {
            in.close();
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else if (header.datatypecode == 10 || header.datatypecode == 11) {
        // Como esta comprimido no se lee byte por byte, se llama a load_rle_data(), retorna falso si se encuentra un
        // error
        if (!load_rle_data(in)) {
            in.close();
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else { // Si es que no encuentra el formato que se esta trabajando entonces retorna false
        in.close();
        std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
        return false;
    }

    /*
        Bit 4 of the image descriptor byte indicates right-to-left pixel ordering if set.
        Bit 5 indicates an ordering of top-to-bottom. Otherwise, pixels are stored in bottom-to-top, left-to-right
       order.

        Los bits de los .tga se cuentan asi (Se cuentan asi debido al manual que dio
        Trueision Inc) 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
        - - - - - - - - - - - - - - -
        0 | 0 | x | x | 0 | 0 | 0 | 0 // Son los bits que determinan el Origen de la imagen
    */

    // Si el header.imagedescriptor tiene el bit 5 NO seteado entonces se hace un flip verticalmente
    if (!(header.imagedescriptor & 0x20)) { // 0x20 == 0b0010'0000
        flip_vertically();
    }

    // Si el header.imagedescriptor tiene el bit 4 seteado entonces se hace un flip hirizontalmente
    if (header.imagedescriptor & 0x10) { // 0x10 == 0b0001'0000
        flip_horizontally();
    }

    // std::cerr funciona como std::cout, pero no hace uso de un buffer
    std::cerr << m_width << "x" << m_height << "/" << m_bytespp * 8 << " bits per pixel\n";
    in.close();
    return true;
}

bool TGAImage::load_rle_data(std::ifstream& in) {
    unsigned long pixelcount = m_width * m_height; // Cantidad de pixeles en la imagen
    unsigned long currentpixel = 0;                // Indice que representa el pixel actual
    unsigned long currentbyte = 0;                 // Indice que representa el byte actual

    TGAColor colorbuffer; // ...

    do {
        /*
            En RLE (Run-Length-Encoded) en los .tga, hay paquetes que se conforman de 2 partes:
            El primer byte representa la cantidad de veces que se repite un pixel, y los siguientes
            la informacion del pixel (color), la cantidad de bytes de esa parte depende de bitsPerPixel.
            Si el primer byte tiene el primer bit seteado (0b1000'0000), se llama Run-length packet y
            los demás bits conforman la cantidad de pixeles a repetirse sumados con 1. Ver el manual de
            Truevision. Por lo tanto 0b1111'1111(0xFF) represeta que el pixel se
            repite 128 veces, y 0b1000'0011(0x83) que se repite 4 veces por ejemplo.
            Se le resto 0b1000'0000(0x80) + 0x01 para determinar la cantidad de veces que se repite un pixel
            Si el primer bit por el contrario es 0, se llama Raw packet(No RLE), los demas
            bits representan la cantidad de pixeles que hay de ahi en adelante.
        */
        unsigned char chunkheader = 0; // Se toma como el Repetition-count-field (y como la veces que se repite)
        chunkheader = in.get();        // Extrae un caracter (un byte)

        if (!in.good()) { // Verifica que ninguna de las flags de error este seteada
            std::cerr << "an error occured while reading the data\n";
            return false;
        }

        if (chunkheader < 128) {                            // True si detecta que el primer bit NO esta seteado
            chunkheader++;                                  // Se le aumenta uno para representar la cantidad de pixeles
            for (int i = 0; i < chunkheader; i++) {         // Bucle que recorre pixel por pixel
                in.read((char*)colorbuffer.raw, m_bytespp); // Se lee la informacion del pixel en el colorbuffer
                if (!in.good()) {
                    std::cerr << "an error occured while reading the header\n";
                    return false;
                }
                for (int t = 0; t < m_bytespp; t++) // Se copia el colorbuffer a m_data
                    m_data[currentbyte++] = colorbuffer.raw[t];
                currentpixel++; // Una vez acabado el bucle, un pixel ya esta guardado en m_data
                if (currentpixel > pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        } else {                // De lo contrario el primer bit esta seteado (se lo considera un Run-length packet)
            chunkheader -= 127; // Se le resta 128 y luego se suma 1 (seria -127) para representar la cantidad
            in.read((char*)colorbuffer.raw, m_bytespp); // Se lee el pixel a repetirse

            if (!in.good()) {
                std::cerr << "an error occured while reading the header\n";
                return false;
            }

            for (int i = 0; i < chunkheader; i++) { // Bucle que se repite la cantidad de pixeles a repetir
                for (int t = 0; t < m_bytespp; t++) // Se copia del colorbuffer a m_data
                    m_data[currentbyte++] = colorbuffer.raw[t];
                currentpixel++; // Ya se leyo un pixel, por lo tanto se le aumenta
                if (currentpixel > pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        }
    } while (currentpixel < pixelcount);
    return true;
}

bool TGAImage::write_tga_file(const char* filename, bool rle) {
    // Revisar el manual de Truevsion
    // Se asignan la variables para el footer
    /*
        Bytes 0-3: The Extension Area Offset
        Bytes 4-7: The Developer Directory Offset
        Bytes 8-23: The Signature
        Byte 24: ASCII Character “.”
        Byte 25: Binary zero string terminator (0x00)
    */
    unsigned char developer_area_ref[4] = { 0, 0, 0, 0 };
    unsigned char extension_area_ref[4] = { 0, 0, 0, 0 };
    unsigned char footer[18] = { 'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O',
                                 'N', '-', 'X', 'F', 'I', 'L', 'E', '.', '\0' }; // byte 8 al 25

    std::ofstream out;                    // Crea el archivo de salida
    out.open(filename, std::ios::binary); // Crea un archivo de nombre filename y lo abre en modo binario

    if (!out.is_open()) {
        std::cerr << "can't open file " << filename << "\n";
        out.close();
        return false;
    }

    TGA_Header header;
    std::memset((void*)&header, 0, sizeof(header)); // Reemplaza con 0x00 cada byte de header (que pesa 18 bytes)

    // Se copian los valores trascendentes guardados de TGAImg al header creado
    header.bitsperpixel = m_bytespp * 8; // m_bytespp << 3;
    header.width = m_width;
    header.height = m_height;
    /*
            0 no image data is present
            1 uncompressed color-mapped image
            2 uncompressed true-color image                         (***)
            3 uncompressed black-and-white (grayscale) image        (***)
            9 run-length encoded color-mapped image
            10 run-length encoded true-color image                  (***)
            11 run-length encoded black-and-white (grayscale) image (***)
    */
    header.datatypecode = (m_bytespp == GRAYSCALE ? (rle ? 11 : 3) : (rle ? 10 : 2));

    // El origen siempre sera la esquina superior inzquierda
    header.imagedescriptor = 0x20; // top-left origin 0b0010'0000

    // Se Empieza a escribir el archivo
    out.write((char*)&header, sizeof(header));
    if (!out.good()) {
        out.close();
        std::cerr << "can't dump the tga file\n";
        return false;
    }

    if (!rle) { // Si no se escribe en RLE, simplemente se copia m_data al archivo
        out.write((char*)m_data, m_width * m_height * m_bytespp);
        if (!out.good()) {
            std::cerr << "can't unload raw data\n";
            out.close();
            return false;
        }
    } else { // De lo contrario se ejecuta un algoritmo para codificar los Run-length packets
        if (!unload_rle_data(out)) {
            out.close();
            std::cerr << "can't unload rle data\n";
            return false;
        }
    }

    out.write((char*)developer_area_ref, sizeof(developer_area_ref));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }

    out.write((char*)extension_area_ref, sizeof(extension_area_ref));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }

    out.write((char*)footer, sizeof(footer));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        out.close();
        return false;
    }
    out.close();
    return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
bool TGAImage::unload_rle_data(std::ofstream& out) {
    const unsigned char max_chunk_length = 128; // Cantidad maxima de pixeles repetidos 0x7F + 0x1
    unsigned long npixels = m_width * m_height; // Cantidad total de pixeles
    unsigned long curpix = 0;                   // Current pixel | pixel actual

    while (curpix < npixels) {
        unsigned long chunkstart = curpix * m_bytespp;
        unsigned long curbyte = curpix * m_bytespp;
        unsigned char run_length = 1; // Cantidad de pixeles repetidos
        bool raw = true; // Bandera que determina si es un Raw packet o un Run-length packet, ver manual de Truevision

        while (curpix + run_length < npixels && run_length < max_chunk_length) {
            bool succ_eq = true; // Successor equal | Si el siguiente pixel es igual

            // Bucle que compara el pixel actual con el siguiente para ver si son iguales
            for (int t = 0; succ_eq && (t < m_bytespp); t++) {
                succ_eq = (m_data[curbyte + t] == m_data[curbyte + t + m_bytespp]);
            }

            curbyte += m_bytespp; // Se pasa al primer byte del siguiente pixel

            if (1 == run_length) { // Este if se da solo en la primera iteracion
                raw = !succ_eq;
            }
            if (raw && succ_eq) { // True cuando los bytes son un Raw-packet y se topa un byte igual al ultimo recorrido
                run_length--;
                break;
            }
            if (!raw && !succ_eq) { // True cuando NO es un Raw-packet y se topa un byte diferente al ultimo recorrido
                break;
            }
            run_length++;
        }

        curpix += run_length;
        out.put(raw ? run_length - 1 : run_length + 128 - 1);

        if (!out.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }

        out.write((char*)(m_data + chunkstart), (raw ? run_length * m_bytespp : m_bytespp));
        if (!out.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
    }
    return true;
}

// TGAColor TGAImage::get(int x, int y) {
//     if (!m_data || x < 0 || y < 0 || x >= m_width || y >= m_height) {
//         return TGAColor();
//     }
//     return TGAColor(m_data + (x + y * m_width) * m_bytespp, m_bytespp);
// }

// bool TGAImage::set(int x, int y, const TGAColor& c) {
//     if (!m_data || x < 0 || y < 0 || x >= m_width || y >= m_height) {
//         // std::cout << "Out of bounds";
//         return false;
//     }
//     std::memcpy(m_data + (x + y * m_width) * m_bytespp, c.raw, m_bytespp);
//     return true;
// }

// int TGAImage::get_bytespp() { return m_bytespp; }

// int TGAImage::get_width() { return m_width; }

// int TGAImage::get_height() { return m_height; }

bool TGAImage::flip_horizontally() {
    if (!m_data) // True si m_data es nulo
        return false;
    int half = m_width / 2; // Pixel de la mitad
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < m_height; j++) {
            // Se intercambian los pixeles del eje horizontal (i)
            TGAColor c1 = get(i, j);
            TGAColor c2 = get(m_width - 1 - i, j);

            set(i, j, c2);
            set(m_width - 1 - i, j, c1);
        }
    }
    return true;
}

bool TGAImage::flip_vertically() {
    if (!m_data) // True si m_data es un puntero nulo
        return false;

    unsigned long bytes_per_line = m_width * m_bytespp;      // Bytes por linea
    unsigned char* line = new unsigned char[bytes_per_line]; // Funciona como un buffer
    int half = m_height / 2;
    for (int j = 0; j < half; j++) {
        unsigned long l1 = j * bytes_per_line;
        unsigned long l2 = (m_height - 1 - j) * bytes_per_line;

        // Funcioa como un cambio de variable con una variable auxiliar
        std::memmove((void*)line, (void*)(m_data + l1), bytes_per_line);
        std::memmove((void*)(m_data + l1), (void*)(m_data + l2), bytes_per_line);
        std::memmove((void*)(m_data + l2), (void*)line, bytes_per_line);
    }
    delete[] line;
    return true;
}

unsigned char* TGAImage::buffer() { return m_data; }

void TGAImage::clear() { memset((void*)m_data, 0, m_width * m_height * m_bytespp); }

bool TGAImage::scale(int w, int h) {
    if (w <= 0 || h <= 0 || !m_data) // Controla si la altura o el ancho es menor a 0, o si m_data es nulo
        return false;

    unsigned char* tdata = new unsigned char[w * h * m_bytespp]; // como m_data pero con las nuevas dimensionesX
    int nscanline = 0;                                           // New scan line
    int oscanline = 0;                                           // Original scan line
    int erry = 0;                                                // Error en Y
    unsigned long nlinebytes = w * m_bytespp;                    // cantidad de bytes de la linea nueva
    unsigned long olinebytes = m_width * m_bytespp;              // cantidad de bytes de la linea original

    for (int j = 0; j < m_height; j++) { // Bucle que recorre la altura
        // int errx = m_width - w;
        int errx = 0;

        int nx = -m_bytespp;
        int ox = -m_bytespp;

        for (int i = 0; i < m_width; i++) { // Bucle que recorre el ancho

            ///////////////////////////
            // if (i == 0)
            //     errx += w - m_width;
            ///////////////////////////
            ox += m_bytespp;
            errx += w;
            while (errx >= (int)m_width) {
                errx -= m_width;
                nx += m_bytespp;
                std::memcpy(tdata + nscanline + nx, m_data + oscanline + ox, m_bytespp);
            }
        }

        erry += h;
        oscanline += olinebytes;
        while (erry >= (int)m_height) {
            if (erry >= (int)m_height << 1) // it means we jump over a scanline
                std::memcpy(tdata + nscanline + nlinebytes, tdata + nscanline, nlinebytes);
            erry -= m_height;
            nscanline += nlinebytes;
        }
    }

    delete[] m_data;
    m_data = tdata;
    m_width = w;
    m_height = h;
    return true;
}