#ifndef ITFPLAYER_ANDROIDVDECODE_H
#define ITFPLAYER_ANDROIDVDECODE_H

#include "BaseVDecode.h"
#include "FFVDecode.h"

class AndroidVDecode : protected BaseVDecode {

public:
    AndroidVDecode();

    void Source(char *url);

protected:
    int Encode();//-2.unknown -1.finish 0.continue

private:
    FFVDecode *m_decoder = nullptr;

};

#endif //ITFPLAYER_ANDROIDVDECODE_H