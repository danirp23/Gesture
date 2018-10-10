//
// Created by Daniela Rodriguez on 21/08/2018.
//

#ifndef GESTURE_PUNTOS_H
#define GESTURE_PUNTOS_H


#include <dlib/image_processing.h>
#include "FImage.h"
#include <fstream>
#include <streambuf>

namespace uac{
    class Puntos{
        private:

        public:
            void puntillos(int nob,FImage& img, std::vector<cv::Point>& puntos,char* pFileContent, size_t fileLength, int x, int y, int width, int height );

    };
    struct membuf : std::streambuf
    {
        membuf(char* begin, char* end)
        {
            this->setg(begin, begin, end);
        }

    protected:
        virtual pos_type seekoff(off_type off,
                                 std::ios_base::seekdir dir,
                                 std::ios_base::openmode which = std::ios_base::in) {
            if (dir == std::ios_base::cur)
                gbump(off);
            else if (dir == std::ios_base::end)
                setg(eback(), egptr() + off, egptr());
            else if (dir == std::ios_base::beg)
                setg(eback(), eback() + off, egptr());
            return gptr() - eback();
        }
    };


}

#endif //GESTURE_PUNTOS_H
