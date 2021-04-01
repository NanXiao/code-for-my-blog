#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#define CAFILE "/etc/ssl/cacert.pem"
#define CAPATH "/etc/ssl/certs/"

#define BUFSIZE 1024

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: ./client host:port\n");
    exit(1);
  }
  int ret = 1;

  const char* hostport = argv[1];
  fprintf(stderr, "Trying a connection to %s...\n", hostport);

  char *err_msg = NULL;
  SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
  if (ctx == NULL) {
    err_msg = "SSL_CTX_new";
    goto END;
  }

  if (!SSL_CTX_load_verify_locations(ctx, CAFILE, CAPATH)) {
    err_msg = "SSL_CTX_load_verify_locations";
    goto END;
  }

  BIO* bio = BIO_new_ssl_connect(ctx);
  if (bio == NULL) {
    err_msg = "BIO_new_ssl_connect";
    goto END;
  }
  
  BIO_set_conn_hostname(bio, hostport);
  if (BIO_do_connect(bio) <= 0) {
    err_msg = "BIO_do_connect";
    goto END;
  }

  SSL* ssl = NULL;
  BIO_get_ssl(bio, &ssl);
  long verify_err = SSL_get_verify_result(ssl);
  if (verify_err != X509_V_OK) {
    fprintf(
      stderr,
      "Certificate verification error (error code is %ld:%s) but continuing...\n",
      verify_err,
      X509_verify_cert_error_string(verify_err));
  }

  char request[BUFSIZE];
  snprintf(
    request,
    sizeof(request),
    "GET / HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\n\r\n",
    hostport);
  BIO_puts(bio, request);

  while (1) {
    char response[BUFSIZE] = {0};
    int n = BIO_read(bio, response, sizeof(response));
    if (n <= 0) {
      break;
    }
    puts(response);
  }
  
  ret = 0;

END:
  SSL_CTX_free(ctx);
  BIO_free_all(bio);
  if (ret != 0) {
    if (ERR_peek_error() == 0) {
        perror(err_msg);
    } else {
        ERR_print_errors_fp(stderr);
    }
  }
  return ret;
}

