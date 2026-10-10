#pragma once

#include <zce/zce_handler.h>
#include <string>

typedef struct bio_st BIO;
typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct x509_store_ctx_st X509_STORE_CTX;

namespace zce {

/// How a TLS client checks the server it has connected to.
///
/// The checks are those of HTTPS: the certificate chain the server presents must lead to one of
/// the trust anchors, every certificate in it must be within its validity period, and the server's
/// certificate must be issued to the server name. The name is matched against the certificate's
/// subjectAltName DNS and IP entries, and against its subject CN only when it has no DNS entry;
/// a wildcard matches only a whole leftmost label ("*.example.com").
struct TlsClientOptions {
    enum Verify {
        /// Accept whatever certificate the server presents. The connection is encrypted, but anyone
        /// who can intercept it can pose as the server. This is what a plain "ssl" flag gives.
        VERIFY_NONE = 0,
        /// Run the checks and log the outcome, but keep the connection whatever it is: a warning
        /// with the reasons for a server that would be refused, an info line for one that passes.
        /// For rolling verification out without cutting off servers that would fail it.
        VERIFY_AUDIT = 1,
        /// Run the checks; a server that fails them is disconnected during the handshake, before
        /// anything is sent to it.
        VERIFY_REQUIRED = 2,
    };

    Verify verify = VERIFY_REQUIRED;

    /// Trust anchors as PEM text: one or more certificates. Each one is trusted as it is, so it may
    /// be a root CA, an intermediate CA or the server's own certificate.
    std::string ca_pem;

    /// Trust anchors read from a PEM file, as with ca_pem. The file is read once per process, by
    /// the first connection with these options; a change to it takes a restart to apply.
    /// When both are set, both are trusted.
    /// When neither is, the system's trust store is used instead: OpenSSL's default locations
    /// (SSL_CERT_FILE, SSL_CERT_DIR or its built-in directory) and, on Windows with OpenSSL 3.2
    /// or later, the system's "ROOT" certificate store.
    std::string ca_file;

    /// The DNS name or IP address the server's certificate must be issued to. A DNS name is also
    /// sent as SNI. Empty: the host the client dialed.
    std::string server_name;
};

}  // namespace zce

class zce_ssl : public zce::IStream
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

    zce::RefBlock dblock_;

    /* SSL reads from, we write to. */
    //zce_dblock read_dblock_;
    BIO *read_bio_;

    /* SSL writes to, we read from. */
    //zce_dblock write_dblock_;
    BIO *write_bio_;

    // zce::TlsClientOptions::Verify; VERIFY_NONE for a server and for the legacy constructor
    int verify_;

    // the name the server's certificate is checked against, for the log
    std::string server_name_;

    // what the checks found, filled in by verify_peer() during the handshake
    std::string verify_errors_;

    std::string peer_certificate_;

    static int verify_peer(int ok, X509_STORE_CTX* store_ctx);

    // logs what the checks found once the handshake is done; false if the server must be refused
    bool report_verified();

    void report_handshake_failure();

    zce_ssl::sslstatus get_sslstatus(int n);

    zce_ssl::sslstatus do_ssl_handshake();

    zce_ssl::sslstatus do_ssl_shutdown();

    zce_ssl::sslstatus do_check_write_bio();

    zce_ssl::sslstatus do_check_read_bio();

public:

    zce_ssl(bool isserver, const char* n, const char* verifycrt, const char* cert, const char* key);

    /// A TLS client that checks the server as opts says.
    ///
    /// @param dialed_host the host the client connects to; the server name to check when
    ///        opts.server_name is empty.
    ///
    /// Under VERIFY_REQUIRED, trust anchors that cannot be loaded, or an empty server name, fail
    /// the connection as soon as it opens. Under VERIFY_AUDIT they are logged, and the connection
    /// goes ahead unchecked, still sending a DNS server name as SNI.
    zce_ssl(const zce::TlsClientOptions& opts, const char* dialed_host);

    ~zce_ssl();

    static SSL_CTX* init_ssl_ctx(bool isserver, const char* verifycrt, const char* cert, const char* key);

    void on_open(bool passive, const zce_sockaddr_t& remote) override;

    void on_read(zce::RefBlock& dblock, const zce::Any&) override;

    void close() override;

    int write(zce::RefBlock& dblock, ERV_ISTREAM_WRITEOPT opt) override;
};
