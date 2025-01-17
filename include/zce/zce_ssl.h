#pragma once

#include <zce/zce_handler.h>

typedef struct bio_st BIO;
typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;

class zce_ssl : public zce_istream
{
    enum _tls_state {
        STATE_INIT = 0x0,
        STATE_HANDSHAKING = 0x1,
        STATE_IO = 0x2, //read or write mode
        STATE_CLOSING = 0x4 // This means closed state also
    };

    enum sslstatus { SSLSTATUS_OK, SSLSTATUS_WANT_READ, SSLSTATUS_WANT_WRITE, SSLSTATUS_FAIL };

    SSL* ssl_;

    bool is_server_;

    zce_dblock dblock_;

    /* SSL reads from, we write to. */
    //zce_dblock read_dblock_;
    BIO *read_bio_;

    /* SSL writes to, we read from. */
    //zce_dblock write_dblock_;
    BIO *write_bio_;

    zce_ssl::sslstatus get_sslstatus(int n);

    zce_ssl::sslstatus do_ssl_handshake();

    zce_ssl::sslstatus do_ssl_shutdown();

    zce_ssl::sslstatus do_check_write_bio();

    zce_ssl::sslstatus do_check_read_bio();

public:

    zce_ssl(bool isserver, const char* n, const char* verifycrt, const char* cert, const char* key);

    ~zce_ssl();

    static SSL_CTX* init_ssl_ctx(bool isserver, const char* verifycrt, const char* cert, const char* key);

    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_read(zce_dblock& dblock, const zce_any&) override;

    void close() override;

    int  write(zce_dblock& dblock, ERV_ISTREAM_WRITEOPT opt) override;
};
