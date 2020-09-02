#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <stdbool.h>
#include <stdio.h>

static void print(unsigned char *title, unsigned char *output, size_t output_len)
{
	printf("%s(%zu): ", title, output_len);
	for (size_t i = 0; i < output_len; i++) {
		printf("%02x", output[i]);
	}
	printf("\n");
}

static bool hkdf_extract(
		EVP_PKEY_CTX *pctx,
		unsigned char *salt,
		size_t salt_len,
		unsigned char *key,
		size_t key_len,
		unsigned char *secret,
		size_t *secret_len)
{
	if ((EVP_PKEY_derive_init(pctx) <= 0) ||
		 (EVP_PKEY_CTX_hkdf_mode(pctx, EVP_PKEY_HKDEF_MODE_EXTRACT_ONLY) <= 0) ||
		 (EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256()) <= 0) ||
		 (EVP_PKEY_CTX_set1_hkdf_salt(pctx, salt, salt_len) <= 0) ||
		 (EVP_PKEY_CTX_set1_hkdf_key(pctx, key, key_len) <= 0) ||
		 (EVP_PKEY_derive(pctx, secret, secret_len) <= 0)) {
		return false;
	} else {
		return true;
	}
}

static bool hdfk_expand(
		EVP_PKEY_CTX *pctx,
		unsigned char *key,
		size_t key_len,
		unsigned char *label,
		size_t label_len,
		unsigned char *secret,
		size_t *secret_len)
{
	if ((EVP_PKEY_derive_init(pctx) <= 0) ||
		(EVP_PKEY_CTX_hkdf_mode(pctx, EVP_PKEY_HKDEF_MODE_EXPAND_ONLY) <= 0) ||
		(EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256()) <= 0) ||
		(EVP_PKEY_CTX_set1_hkdf_key(pctx, key, key_len) <= 0) ||
		(EVP_PKEY_CTX_add1_hkdf_info(pctx, label, label_len) <= 0) ||
		(EVP_PKEY_derive(pctx, secret, secret_len) <= 0)) {
		return false;
	} else {
		return true;
	}
}

int main(void)
{
	int ret_val = 1;

	EVP_PKEY_CTX * pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
	if (pctx == NULL)
	{
		fprintf(stderr, "Error in initialising, exit\n");
		goto END;
	}

	unsigned char initial_secret[1024];
	size_t initial_secret_len = sizeof(initial_secret);
	unsigned char salt[] = {
			0xaf, 0xbf, 0xec, 0x28, 0x99, 0x93, 0xd2,
			0x4c, 0x9e, 0x97, 0x86, 0xf1, 0x9c, 0x61,
			0x11, 0xe0, 0x43, 0x90, 0xa8, 0x99};
	unsigned char cid[] = {0x83, 0x94, 0xc8, 0xf0, 0x3e, 0x51, 0x57, 0x08};

	if (hkdf_extract(pctx, salt, sizeof(salt), cid, sizeof(cid), initial_secret, &initial_secret_len))
	{
		print("initial_secret", initial_secret, initial_secret_len);
	}
	else
	{
		fprintf(stderr, "Error in extracting initial secret, exit\n");
		goto END;
	}

	unsigned char client_in_secret[1024];
	size_t client_in_secret_len = 32;
	unsigned char client_in_label[] = {
		0x00, 0x20, 0x0f, 0x74, 0x6c, 0x73, 0x31, 0x33,
		0x20, 0x63, 0x6c, 0x69, 0x65, 0x6e, 0x74, 0x20,
		0x69, 0x6e, 0x00};

	if (hdfk_expand(
		pctx,
		initial_secret,
		initial_secret_len,
		client_in_label,
		sizeof(client_in_label),
		client_in_secret,
		&client_in_secret_len))
	{
		print("client_in_secret", client_in_secret, client_in_secret_len);
	}
	else
	{
		fprintf(stderr, "Error in extracting client in secret, exit\n");
		goto END;
	}

	unsigned char quic_secret[1024];

	unsigned char quic_key_label[] = {
		0x00, 0x10, 0x0e, 0x74, 0x6c, 0x73, 0x31, 0x33,
		0x20, 0x71, 0x75, 0x69, 0x63, 0x20, 0x6b, 0x65,
		0x79, 0x00};
	size_t quic_secret_len = 16;
	if (hdfk_expand(
		pctx,
		client_in_secret,
		client_in_secret_len,
		quic_key_label,
		sizeof(quic_key_label),
		quic_secret,
		&quic_secret_len))
	{
		print("quic_key", quic_secret, quic_secret_len);
	}
	else
	{
		fprintf(stderr, "Error in extracting quic key, exit\n");
		goto END;
	}

	unsigned char quic_iv_label[] = {
		0x00, 0x0c, 0x0d, 0x74, 0x6c, 0x73, 0x31, 0x33,
		0x20, 0x71, 0x75, 0x69, 0x63, 0x20, 0x69, 0x76,
		0x00};
	quic_secret_len = 12;
	if (hdfk_expand(
		pctx,
		client_in_secret,
		client_in_secret_len,
		quic_iv_label,
		sizeof(quic_iv_label),
		quic_secret,
		&quic_secret_len))
	{
		print("quic_iv", quic_secret, quic_secret_len);
	}
	else
	{
		fprintf(stderr, "Error in extracting quic iv, exit\n");
		goto END;
	}

	unsigned char quic_hp_label[] = {
		0x00, 0x10, 0x0d, 0x74, 0x6c, 0x73, 0x31, 0x33,
		0x20, 0x71, 0x75, 0x69, 0x63, 0x20, 0x68, 0x70,
		0x00};
	quic_secret_len = 16;
	if (hdfk_expand(
		pctx,
		client_in_secret,
		client_in_secret_len,
		quic_hp_label,
		sizeof(quic_hp_label),
		quic_secret,
		&quic_secret_len))
	{
		print("quic_hp", quic_secret, quic_secret_len);
	}
	else
	{
		fprintf(stderr, "Error in extracting quic hp, exit\n");
		goto END;
	}

	ret_val = 0;

END:
	EVP_PKEY_CTX_free(pctx);
	return ret_val;
}
